#include <iostream>
#include <string>
#include <vector>
#include <thread>

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "boost/asio.hpp"
#include "boost/bind/bind.hpp"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "exchangeInfoClass.h"
#include "example/common/root_certificates.hpp"
#include <boost/asio/ssl.hpp>

// global variables to store base url and endpoints info
std::string spotExchangeBaseUrl, usdFutureExchangeBaseUrl, coinFutureExchangeBaseUrl;
std::string spotExchangeEndpoint, usdFutureEndpoint, coinFutureEndpoint;
int requestInterval;

// struct to store logging info from config.json
struct logsInfo {
    std::string level;
    bool file;
    bool console;
} logsConfig;

// instance of class excahngeInfo defined in exchangeInfoClass.h, stores data of all endpoints in respective map
exchangeInfo binanceExchange;

// read config.json for logging, request url, request interval
void readConfig(std::string configFile) {

    spdlog::info("Reading config file: {}", configFile);
    // open config.json
    FILE* fp = fopen(configFile.c_str(), "r"); 
    
    // throw error if file not opened
    if (!fp) { 
        spdlog::error("Error: unable to open file");
    } 

    rapidjson::Document doc;

    char buffer[65536];
    rapidjson::FileReadStream is(fp, buffer, sizeof(buffer));

    // parse json data
    doc.ParseStream(is);

    // store base url of each endpoint
    spotExchangeBaseUrl = doc["exchange_base_url"]["spot_exchange_info_base_uri"].GetString();
    usdFutureExchangeBaseUrl = doc["exchange_base_url"]["usd_futures_exchange_info_base_uri"].GetString();
    coinFutureExchangeBaseUrl = doc["exchange_base_url"]["coin_futures_exchange_info_base_uri"].GetString();

    // store each endpoint
    spotExchangeEndpoint = doc["exchange_endpoints"]["spot_exchange_info_uri"].GetString();
    usdFutureEndpoint = doc["exchange_endpoints"]["usd_futures_exchange_info_uri"].GetString();
    coinFutureEndpoint = doc["exchange_endpoints"]["coin_futures_exchange_info_uri"].GetString();
    requestInterval = doc["request_interval"].GetInt();
    
    // store logging level, file enable, console enable
    logsConfig.level = doc["logging"]["level"].GetString();
    logsConfig.file = doc["logging"]["file"].GetBool();
    logsConfig.console = doc["logging"]["console"].GetBool();

    // close file
    fclose(fp); 

    spdlog::info("Config file loaded successfully");
}

// Function to fetch data of all 3 endpoints
void fetchAll(const boost::system::error_code& /*e*/, boost::asio::steady_timer* t, boost::asio::io_context& ioc){
    
    spdlog::info("Fetching all data");  

    // Create three threads to fetch data from each endpoint  
    std::thread spotThread (fetchData, std::ref(binanceExchange), spotExchangeBaseUrl, spotExchangeEndpoint);
    std::thread usdFutureThread (fetchData, std::ref(binanceExchange), usdFutureExchangeBaseUrl, usdFutureEndpoint);
    std::thread coinFutureThread (fetchData, std::ref(binanceExchange), coinFutureExchangeBaseUrl, coinFutureEndpoint);

    // Wait for each thread to finish fetching data
    spotThread.join();
    usdFutureThread.join();
    coinFutureThread.join();

    // Set the timer to expire in 60 seconds and wait for next fetch
    t->expires_at(t->expiry() + boost::asio::chrono::seconds(35));
    t->async_wait(boost::bind(fetchAll, boost::asio::placeholders::error, t, std::ref(ioc)));

    spdlog::info("Fetch all data completed");    
}

void setSpdLogs(){

    // Create a vector of sinks
    std::vector<spdlog::sink_ptr> sinks;

    // Add console sink if enabled
    if (logsConfig.console) {
        sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
        spdlog::info("Console logging enabled");
    }

    // Add file sink if enabled
    if (logsConfig.file) {
        sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/logfile.log", true));
        spdlog::info("File logging enabled");
    }

    // Create logger
    auto logger = std::make_shared<spdlog::logger>("BinanceExchangeLogs", begin(sinks), end(sinks));

    // set level
    logger->set_level(spdlog::level::from_str(logsConfig.level));
    
    // register logger
    spdlog::register_logger(logger);
    spdlog::set_default_logger(logger);
    logger->flush_on(spdlog::level::from_str(logsConfig.level));

    spdlog::info("Logger setup completed");
}


int main() {

    // read configuration file
    readConfig("config.json");
    
    // set up logging system
    setSpdLogs();

    spdlog::info("Logging Level: {}", logsConfig.level);
    spdlog::info("Logging to File: {}", logsConfig.file ? "Enabled" : "Disabled");
    spdlog::info("Logging to Console: {}", logsConfig.console ? "Enabled" : "Disabled");


    spdlog::info("Spot Exchange Info base URI: {}", spotExchangeBaseUrl);
    spdlog::info("USD Futures Exchange Info base URI: {}", usdFutureExchangeBaseUrl);
    spdlog::info("Coin Futures Exchange Info base URI: {}", coinFutureExchangeBaseUrl);
    spdlog::info("Spot Exchange Info URI: {}", spotExchangeEndpoint);
    spdlog::info("USD Futures Exchange Info URI: {}", usdFutureEndpoint);
    spdlog::info("Coin Futures Exchange Info URI: {}", coinFutureEndpoint);
    spdlog::info("Request Interval: {} seconds", requestInterval);

    // thread to run the readQuery function
    std::thread readQueryThread(readQuery, std::ref(binanceExchange));

    // The io_context is required for all I/O
    boost::asio::io_context io;

    // The SSL context is required, and holds certificates
    boost::asio::ssl::context ctx{ssl::context::tlsv12_client};

    // This holds the root certificate used for verification
    load_root_certificates(ctx);

    // Verify the remote server's certificate
    ctx.set_verify_mode(ssl::verify_peer);

    // timer to fetch data every 60 sec
    boost::asio::steady_timer t(io, boost::asio::chrono::seconds(35));

    // call back fetchAll function when timer expires
    t.async_wait(boost::bind(fetchAll, boost::asio::placeholders::error, &t, std::ref(io)));

    // Run IO context
    io.run();

   // Wait for the readQuery thread to finish
    readQueryThread.join();
    
    // // // std::thread ioThread(runIoContext, std::ref(io));

    // // // ioThread.join();


    return 0;
}

