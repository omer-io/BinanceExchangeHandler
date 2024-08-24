#include <iostream>
#include <string>
#include <vector>
#include <thread>

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "boost/asio.hpp"
#include "boost/bind/bind.hpp"

// functions declared here are defined in src/getApiData.cpp
void spotData(std::string, std::string);
void usdFutureData(std::string, std::string);
void coinFutureData(std::string, std::string);
void readQuery();

// global variables to store base url and endpoints info
std::string spotExchangeBaseUrl, usdFutureExchangeBaseUrl, coinFutureExchangeBaseUrl;
std::string spotExchangeEndpoint, usdFutureEndpoint, coinFutureEndpoint;
int request_interval;

// struct to store logging info from config.json
struct logsInfo {
    std::string level;
    bool file;
    bool console;
} logsConfig;

// read config.json for logging, request url, request interval
void readConfig(std::string configFile, rapidjson::Document &doc) {

    // open config.json
    FILE* fp = fopen(configFile.c_str(), "r"); 
    
    // throw error if file not opened
    if (!fp) { 
        std::cerr << "Error: unable to open file" << std::endl; 
    } 

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
    request_interval = doc["request_interval"].GetInt();
    
    // store logging level, file enable, console enable
    logsConfig.level = doc["logging"]["level"].GetString();
    logsConfig.file = doc["logging"]["file"].GetBool();
    logsConfig.console = doc["logging"]["console"].GetBool();

    // close file
    fclose(fp); 

}


void fetchAll(const boost::system::error_code& /*e*/, boost::asio::steady_timer* t){
    std::thread spotThread (spotData, spotExchangeBaseUrl, spotExchangeEndpoint);
    std::thread usdFutureThread (usdFutureData, usdFutureExchangeBaseUrl, usdFutureEndpoint);
    std::thread coinFutureThread (coinFutureData, coinFutureExchangeBaseUrl, coinFutureEndpoint);

    spotThread.join();
    usdFutureThread.join();
    coinFutureThread.join();

    t->expires_at(t->expiry() + boost::asio::chrono::seconds(20));
    t->async_wait(boost::bind(fetchAll, boost::asio::placeholders::error, t));

    
}

// run ioc.run() in a thread
// void runIoContext(boost::asio::io_context& io) {
//     io.run();
// }

int main() {

    rapidjson::Document doc;
    readConfig("/home/omer/training/BinanceExchangeHandler/config.json", doc);

    std::cout << "Spot Exchange Info base URI: " << spotExchangeBaseUrl << std::endl;
    std::cout << "USD Futures Exchange Info base URI: " << usdFutureExchangeBaseUrl << std::endl;
    std::cout << "Coin Futures Exchange Info base URI: " << coinFutureExchangeBaseUrl << std::endl;
    std::cout << "Spot Exchange Info URI: " << spotExchangeEndpoint << std::endl;
    std::cout << "USD Futures Exchange Info URI: " << usdFutureEndpoint << std::endl;
    std::cout << "Coin Futures Exchange Info URI: " << coinFutureEndpoint << std::endl;
    std::cout << "Request Interval: " << request_interval << " seconds" << std::endl;

    std::cout << "Logging Level: " << logsConfig.level << std::endl;
    std::cout << "Logging to File: " << (logsConfig.file ? "Enabled" : "Disabled") << std::endl;
    std::cout << "Logging to Console: " << (logsConfig.console ? "Enabled" : "Disabled") << std::endl;

    std::thread readQueryThread(readQuery);

    boost::asio::io_context io;

    boost::asio::steady_timer t(io, boost::asio::chrono::seconds(20));

    t.async_wait(boost::bind(fetchAll, boost::asio::placeholders::error, &t));

    io.run();

    readQueryThread.join();
    
    // // // std::thread ioThread(runIoContext, std::ref(io));
    // // //     std::cout << "hello";
    // // // ioThread.join();


    return 0;
}

