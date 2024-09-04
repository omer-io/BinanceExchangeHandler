#include <thread>

#include "boost/asio.hpp"
#include "boost/bind/bind.hpp"
#include "spdlog/spdlog.h"
#include "example/common/root_certificates.hpp"
#include "boost/asio/ssl.hpp"
#include "BinanceExchange.h"

// Function to fetch data of all 3 endpoints
void fetchAll(exchangeInfo& binanceExchange, urlInfo& urlConfig, const boost::system::error_code& /*e*/, boost::asio::steady_timer* t, boost::asio::io_context& ioc, boost::asio::ssl::context& ctx){
    
    spdlog::info("Fetching all data");  

    // Call fetch data func to fetch data from each endpoint  
    binanceExchange.fetchData(urlConfig, ioc, ctx);

    // Set the timer to expire in 60 seconds and wait for next fetch
    t->expires_at(t->expiry() + boost::asio::chrono::seconds(urlConfig.requestInterval));
    t->async_wait(boost::bind(fetchAll, std::ref(binanceExchange), std::ref(urlConfig), boost::asio::placeholders::error, t, std::ref(ioc), std::ref(ctx)));

    spdlog::info("Fetch all data completed");    
}

int main() {

    // instance of class excahngeInfo defined in exchangeInfoClass.h, stores data of all endpoints in respective map
    exchangeInfo binanceExchange;

    urlInfo urlConfig;
    logsInfo logsConfig;

    // read configuration file
    binanceExchange.readConfig("config.json", urlConfig, logsConfig);
    
    // set up logging system
    binanceExchange.setSpdLogs(logsConfig);

    spdlog::info("Logging Level: {}", logsConfig.level);
    spdlog::info("Logging to File: {}", logsConfig.file ? "Enabled" : "Disabled");
    spdlog::info("Logging to Console: {}", logsConfig.console ? "Enabled" : "Disabled");


    spdlog::info("Spot Exchange Info base URI: {}", urlConfig.spotExchangeBaseUrl);
    spdlog::info("USD Futures Exchange Info base URI: {}", urlConfig.usdFutureExchangeBaseUrl);
    spdlog::info("Coin Futures Exchange Info base URI: {}", urlConfig.coinFutureExchangeBaseUrl);
    spdlog::info("Spot Exchange Info URI: {}", urlConfig.spotExchangeEndpoint);
    spdlog::info("USD Futures Exchange Info URI: {}", urlConfig.usdFutureEndpoint);
    spdlog::info("Coin Futures Exchange Info URI: {}", urlConfig.coinFutureEndpoint);
    spdlog::info("Request Interval: {} seconds", urlConfig.requestInterval);

    // thread to run the readQuery function
    std::thread readQueryThread(&exchangeInfo::readQuery, &binanceExchange);

    // The io_context is required for all I/O
    boost::asio::io_context io;

    // The SSL context is required, and holds certificates
    boost::asio::ssl::context ctx{ssl::context::tlsv12_client};

    // This holds the root certificate used for verification
    load_root_certificates(ctx);

    // Verify the remote server's certificate
    ctx.set_verify_mode(ssl::verify_peer);

    // timer to fetch data every 60 sec
    boost::asio::steady_timer t(io, boost::asio::chrono::seconds(urlConfig.requestInterval));

    // call back fetchAll function when timer expires
    t.async_wait(boost::bind(fetchAll, std::ref(binanceExchange), std::ref(urlConfig), boost::asio::placeholders::error, &t, std::ref(io), std::ref(ctx)));

    // Run IO context
    io.run();

   // Wait for the readQuery thread to finish
    readQueryThread.join();
    
    return 0;
}

