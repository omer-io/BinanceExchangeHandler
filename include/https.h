#include <cstdlib>
#include <iostream>
#include <memory>

#include "example/common/root_certificates.hpp"
#include "boost/beast/core.hpp"
#include "boost/beast/http.hpp"
#include "boost/beast/version.hpp"
#include "boost/asio/ssl.hpp"
#include "boost/asio/strand.hpp"
#include "BinanceExchange.h"
#include "rapidjson/document.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

void processResponse(http::response<http::string_body>&, exchangeInfo*, std::string);

// Report a failure
void fail(beast::error_code, char const*);

// http::response<http::string_body> result;

// Performs an HTTP GET and prints the response
class session : public std::enable_shared_from_this<session>
{
    tcp::resolver resolver_;
    ssl::stream<beast::tcp_stream> stream_;
    beast::flat_buffer buffer_; // (Must persist between reads)
    http::request<http::empty_body> req_;
    http::response<http::string_body> res_;

public:
    exchangeInfo* binanceExchangeInfo; 
    std::string baseUrl;
    explicit
    session(net::any_io_executor, ssl::context&, exchangeInfo&, std::string);

    // Start the asynchronous operation
    void run(char const*, char const*, char const*, int);

    void on_resolve(beast::error_code, tcp::resolver::results_type);

    void on_connect(beast::error_code, tcp::resolver::results_type::endpoint_type);

    void on_handshake(beast::error_code);

    void on_write(beast::error_code, std::size_t);

    void on_read(beast::error_code, std::size_t);

    void on_shutdown(beast::error_code);
};

