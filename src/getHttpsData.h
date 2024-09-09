#ifndef getHttpsData_H
#define getHttpsData_H

#include "example/common/root_certificates.hpp"
#include "boost/beast/core.hpp"
#include "boost/beast/http.hpp"
#include "boost/beast/version.hpp"
#include "boost/asio/ssl.hpp"
#include "boost/asio/strand.hpp"
#include "BinanceExchange.h"

// Performs an HTTP GET and prints the response
class session : public std::enable_shared_from_this<session>
{
    private: 
    boost::asio::ip::tcp::resolver _resolver;
    ssl::stream<boost::beast::tcp_stream> _stream;
    boost::beast::flat_buffer _buffer; // (Must persist between reads)
    boost::beast::http::request<boost::beast::http::empty_body> _req;
    boost::beast::http::response<boost::beast::http::string_body> _res;
    exchangeInfo* _binanceExchangeInfo; 
    std::string _baseUrl;
    urlInfo _baseUrls;

    void onResolve(boost::beast::error_code, boost::asio::ip::tcp::resolver::results_type);

    void onConnect(boost::beast::error_code, boost::asio::ip::tcp::resolver::results_type::endpoint_type);

    void onHandshake(boost::beast::error_code);

    void onWrite(boost::beast::error_code, std::size_t);

    void onRead(boost::beast::error_code, std::size_t);
    
    void processResponse(boost::beast::http::response<boost::beast::http::string_body>&, exchangeInfo*, std::string, urlInfo&);

    void onShutdown(boost::beast::error_code);

    // Report a failure
    void fail(boost::beast::error_code, char const*);

public:
    session(boost::asio::any_io_executor, boost::asio::ssl::context&, exchangeInfo*, urlInfo&);

    // Start the asynchronous operation
    void get(char const*, char const*, char const*, int);

};

#endif // getHttpsData_H