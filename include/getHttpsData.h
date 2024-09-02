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

void processResponse(boost::beast::http::response<boost::beast::http::string_body>&, exchangeInfo*, std::string);

// Report a failure
void fail(boost::beast::error_code, char const*);

// boost::beast::http::response<boost::beast::http::string_body> result;

// Performs an HTTP GET and prints the response
class session : public std::enable_shared_from_this<session>
{
    private: 
    boost::asio::ip::tcp::resolver resolver_;
    ssl::stream<boost::beast::tcp_stream> stream_;
    boost::beast::flat_buffer buffer_; // (Must persist between reads)
    boost::beast::http::request<boost::beast::http::empty_body> req_;
    boost::beast::http::response<boost::beast::http::string_body> res_;

    void onResolve(boost::beast::error_code, boost::asio::ip::tcp::resolver::results_type);

    void onConnect(boost::beast::error_code, boost::asio::ip::tcp::resolver::results_type::endpoint_type);

    void onHandshake(boost::beast::error_code);

    void onWrite(boost::beast::error_code, std::size_t);

    void onRead(boost::beast::error_code, std::size_t);

    void onShutdown(boost::beast::error_code);

public:
    exchangeInfo* binanceExchangeInfo; 
    std::string baseUrl;

    session(boost::asio::any_io_executor, boost::asio::ssl::context&, exchangeInfo&, std::string);

    // Start the asynchronous operation
    void run(char const*, char const*, char const*, int);

};

