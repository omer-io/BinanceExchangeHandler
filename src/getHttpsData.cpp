#include "getHttpsData.h"

#include "spdlog/spdlog.h"
#include "rapidjson/document.h"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

session::session(net::any_io_executor ex, ssl::context& ctx, exchangeInfo* exchangeClass, urlInfo& urlConfig) 
: resolver_(ex), stream_(ex, ctx), binanceExchangeInfo(exchangeClass), baseUrls(urlConfig) {}

    // Start the asynchronous operation
void session::get( char const* host, char const* port, char const* target, int version)
{
    baseUrl = host;
    // Set SNI Hostname (many hosts need this to handshake successfully)
    if(! SSL_set_tlsext_host_name(stream_.native_handle(), host))
    {
        beast::error_code ec{static_cast<int>(::ERR_get_error()), net::error::get_ssl_category()};
        spdlog::error("{}", ec.message());
        return;
    }

    // Set up an HTTP GET request message
    req_.version(version);
    req_.method(http::verb::get);
    req_.target(target);
    req_.set(http::field::host, host);
    req_.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    // Look up the domain name
    resolver_.async_resolve(host, port, beast::bind_front_handler(&session::onResolve, shared_from_this()));
}

void session::onResolve(beast::error_code ec, tcp::resolver::results_type results)
{
    if(ec){
        return session::fail(ec, "resolve");
    }
    // Set a timeout on the operation
    beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(40));

    // Make the connection on the IP address we get from a lookup
    beast::get_lowest_layer(stream_).async_connect(results, beast::bind_front_handler(&session::onConnect, shared_from_this()));
}

void session::onConnect(beast::error_code ec, tcp::resolver::results_type::endpoint_type)
{
    if(ec){
        return session::fail(ec, "connect");
    }
    // Perform the SSL handshake
    stream_.async_handshake(ssl::stream_base::client, beast::bind_front_handler(&session::onHandshake, shared_from_this()));
}

void session::onHandshake(beast::error_code ec)
{
    if(ec){
        return session::fail(ec, "handshake");
    }
    // Set a timeout on the operation
    beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(40));

    // Send the HTTP request to the remote host
    http::async_write(stream_, req_, beast::bind_front_handler(&session::onWrite, shared_from_this()));
}

void session::onWrite(beast::error_code ec, std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);

    if(ec){
        return session::fail(ec, "write");
    }
    // Receive the HTTP response
    http::async_read(stream_, buffer_, res_, beast::bind_front_handler(&session::onRead, shared_from_this()));
}

void session::onRead(beast::error_code ec, std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);

    if(ec){
        return session::fail(ec, "read");
    }

    session::processResponse(res_, binanceExchangeInfo, baseUrl, baseUrls);
    spdlog::info("HTTP request completed.");

    // Set a timeout on the operation
    beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(40));

    // Gracefully close the stream
    stream_.async_shutdown(beast::bind_front_handler(&session::onShutdown, shared_from_this()));
}

void session::processResponse(http::response<http::string_body>& result,exchangeInfo* binanceExchange, std::string baseUrl, urlInfo& baseUrls){
        // Parse body of HTTP response as JSON
    rapidjson::Document fullData;
    fullData.Parse(result.body().c_str());

    // Check if parsed data is object and contains symbols array
    if (!fullData.IsObject() || !fullData.HasMember("symbols") || !fullData["symbols"].IsArray()) {
        spdlog::error("Invalid JSON format or missing symbols array.");
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
        if(baseUrl == baseUrls.spotExchangeBaseUrl) { binanceExchange->setSpotSymbol(info.symbol, info); }
        if(baseUrl == baseUrls.usdFutureExchangeBaseUrl) { binanceExchange->setUsdSymbol(info.symbol, info); }
        if(baseUrl == baseUrls.coinFutureExchangeBaseUrl) { binanceExchange->setCoinSymbol(info.symbol, info); } 
   
    }

    // Output total number of symbols found
    if(baseUrl == baseUrls.spotExchangeBaseUrl) { spdlog::info("Total SPOT symbols: {}", binanceExchange->getSpotSymbolsSize()); }
    if(baseUrl == baseUrls.usdFutureExchangeBaseUrl) { spdlog::info("Total usd futures symbols: {}", binanceExchange->getUsdSymbolsSize()); }
    if(baseUrl == baseUrls.coinFutureExchangeBaseUrl) { spdlog::info("Total coin futures symbols: {}", binanceExchange->getCoinSymbolsSize());}
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

