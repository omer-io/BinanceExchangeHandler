#include "getHttpsData.h"

#include "spdlog/spdlog.h"
#include "rapidjson/document.h"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

session::session(net::any_io_executor ex, ssl::context& ctx, exchangeInfo* exchangeClass, urlInfo& urlConfig) 
: _resolver(ex), _stream(ex, ctx), _binanceExchangeInfo(exchangeClass), _baseUrls(urlConfig) {}

    // Start the asynchronous operation
void session::get( char const* host, char const* port, char const* target, int version)
{
    spdlog::trace("Setting up get request for {} ", host);
    _baseUrl = host;
    // Set SNI Hostname (many hosts need this to handshake successfully)
    if(! SSL_set_tlsext_host_name(_stream.native_handle(), host))
    {
        beast::error_code ec{static_cast<int>(::ERR_get_error()), net::error::get_ssl_category()};
        spdlog::error("{}", ec.message());
        return;
    }

    // Set up an HTTP GET request message
    _req.version(version);
    _req.method(http::verb::get);
    _req.target(target);
    _req.set(http::field::host, host);
    _req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    // Look up the domain name
    _resolver.async_resolve(host, port, beast::bind_front_handler(&session::onResolve, shared_from_this()));
}

void session::onResolve(beast::error_code ec, tcp::resolver::results_type results)
{
    if(ec){
        return session::fail(ec, "resolve");
    }
    // Set a timeout on the operation
    beast::get_lowest_layer(_stream).expires_after(std::chrono::seconds(40));

    // Make the connection on the IP address we get from a lookup
    beast::get_lowest_layer(_stream).async_connect(results, beast::bind_front_handler(&session::onConnect, shared_from_this()));
}

void session::onConnect(beast::error_code ec, tcp::resolver::results_type::endpoint_type)
{
    if(ec){
        return session::fail(ec, "connect");
    }
    // Perform the SSL handshake
    _stream.async_handshake(ssl::stream_base::client, beast::bind_front_handler(&session::onHandshake, shared_from_this()));
}

void session::onHandshake(beast::error_code ec)
{
    if(ec){
        return session::fail(ec, "handshake");
    }
    // Set a timeout on the operation
    beast::get_lowest_layer(_stream).expires_after(std::chrono::seconds(40));

    // Send the HTTP request to the remote host
    http::async_write(_stream, _req, beast::bind_front_handler(&session::onWrite, shared_from_this()));
}

void session::onWrite(beast::error_code ec, std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);

    if(ec){
        return session::fail(ec, "write");
    }
    // Receive the HTTP response
    http::async_read(_stream, _buffer, _res, beast::bind_front_handler(&session::onRead, shared_from_this()));
}

void session::onRead(beast::error_code ec, std::size_t bytes_transferred)
{
    spdlog::trace("Reading http data from {} ", _baseUrl);
    boost::ignore_unused(bytes_transferred);

    if(ec){
        return session::fail(ec, "read");
    }

    this->processResponse();
    spdlog::info("HTTP request of {} completed.", _baseUrl);

    // Set a timeout on the operation
    beast::get_lowest_layer(_stream).expires_after(std::chrono::seconds(40));

    // Gracefully close the stream
    _stream.async_shutdown(beast::bind_front_handler(&session::onShutdown, shared_from_this()));
}

void session::processResponse(){
    spdlog::trace("Processing http data from {} ", _baseUrl);
    // Parse body of HTTP response as JSON
    rapidjson::Document fullData;
    fullData.Parse(_res.body().c_str());

    // Check if parsed data is object and contains symbols array
    if (!fullData.IsObject() || !fullData.HasMember("symbols") || !fullData["symbols"].IsArray()) {
        spdlog::error("Invalid JSON format or missing symbols array.");
        return;
    }

    // Access the "symbols" array
    const auto& symbolsArray = fullData["symbols"];

    // iterate over array
    for (const auto& symbol : symbolsArray.GetArray()) {
        symbolInfo info;                                    // structure to hold symbol info
        info.symbol = symbol["symbol"].GetString();         // get symbol name
        info.quoteAsset = symbol["quoteAsset"].GetString(); // get quote asset
        if (symbol.HasMember("status")){
            info.status = symbol["status"].GetString();     // get status for spot and coin future
        }
        if (symbol.HasMember("contractStatus")){
            info.status = symbol["contractStatus"].GetString();     // since usd future api has key contractStatus instead of status
        }           

        // Iterate over filters array
        for (const auto& filter : symbol["filters"].GetArray()) {
            std::string filterType = filter["filterType"].GetString();  // get filter type
            if (filterType == "PRICE_FILTER") {
                info.tickSize = filter["tickSize"].GetString();         // get tick size if filter is PRICE_FILTER
            } else if (filterType == "LOT_SIZE") {
                info.stepSize = filter["stepSize"].GetString();         // get step size if filter is LOT_SIZE
            }
        }

        // Store symbol info in binanceExchange relevant map with symbol name as key
        if(_baseUrl == _baseUrls.spotExchangeBaseUrl) { 
            _binanceExchangeInfo->setSpotSymbol(info.symbol, info); 
        }
        if(_baseUrl == _baseUrls.usdFutureExchangeBaseUrl) { 
            _binanceExchangeInfo->setUsdSymbol(info.symbol, info); 
        }
        if(_baseUrl == _baseUrls.coinFutureExchangeBaseUrl) { 
            _binanceExchangeInfo->setCoinSymbol(info.symbol, info); 
        } 
   
    }

    // Output total number of symbols found
    if(_baseUrl == _baseUrls.spotExchangeBaseUrl) { 
        spdlog::info("Total SPOT symbols: {}", _binanceExchangeInfo->getSpotSymbolsSize()); 
    }
    if(_baseUrl == _baseUrls.usdFutureExchangeBaseUrl) { 
        spdlog::info("Total usd futures symbols: {}", _binanceExchangeInfo->getUsdSymbolsSize()); 
    }
    if(_baseUrl == _baseUrls.coinFutureExchangeBaseUrl) { 
        spdlog::info("Total coin futures symbols: {}", _binanceExchangeInfo->getCoinSymbolsSize());
    }
}

void session::onShutdown(beast::error_code ec)
{
    if(ec != net::ssl::error::stream_truncated){
        return session::fail(ec, "shutdown");
    }
}

// Report a failure
void session::fail(beast::error_code ec, char const* what)
{
    spdlog::error("{}: {}\n", what, ec.message());
}

