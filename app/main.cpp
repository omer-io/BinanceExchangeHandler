#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <boost/date_time/posix_time/posix_time.hpp>


void spotData(std::string, std::string);
void usdFutureData(std::string, std::string);
void coinFutureData(std::string, std::string);

// global variables
std::string spot_exchange_info_base_uri, usd_futures_exchange_info_base_uri, coin_futures_exchange_info_base_uri;
std::string spot_exchange_info_uri, usd_futures_exchange_info_uri, coin_futures_exchange_info_uri;
int request_interval;

void readConfig(std::string configFile, rapidjson::Document &doc) {

    FILE* fp = fopen(configFile.c_str(), "r"); 
    if (!fp) { 
        std::cerr << "Error: unable to open file" << std::endl; 
    } 

    char buffer[65536];
    rapidjson::FileReadStream is(fp, buffer, sizeof(buffer));

    doc.ParseStream(is);

    spot_exchange_info_base_uri = doc["exchange_base_url"]["spot_exchange_info_base_uri"].GetString();
    usd_futures_exchange_info_base_uri = doc["exchange_base_url"]["usd_futures_exchange_info_base_uri"].GetString();
    coin_futures_exchange_info_base_uri = doc["exchange_base_url"]["coin_futures_exchange_info_base_uri"].GetString();

    spot_exchange_info_uri = doc["exchange_endpoints"]["spot_exchange_info_uri"].GetString();
    usd_futures_exchange_info_uri = doc["exchange_endpoints"]["usd_futures_exchange_info_uri"].GetString();
    coin_futures_exchange_info_uri = doc["exchange_endpoints"]["coin_futures_exchange_info_uri"].GetString();
    request_interval = doc["request_interval"].GetInt();

    fclose(fp); 

}


int main() {

    rapidjson::Document doc;
    readConfig("/home/omer/training/BinanceExchangeHandler/config.json", doc);

    std::cout << "Spot Exchange Info base URI: " << spot_exchange_info_base_uri << std::endl;
    std::cout << "USD Futures Exchange Info base URI: " << usd_futures_exchange_info_base_uri << std::endl;
    std::cout << "Coin Futures Exchange Info base URI: " << coin_futures_exchange_info_base_uri << std::endl;
    std::cout << "Spot Exchange Info URI: " << spot_exchange_info_uri << std::endl;
    std::cout << "USD Futures Exchange Info URI: " << usd_futures_exchange_info_uri << std::endl;
    std::cout << "Coin Futures Exchange Info URI: " << coin_futures_exchange_info_uri << std::endl;
    std::cout << "Request Interval: " << request_interval << " seconds" << std::endl;

    std::thread spotThread (spotData, spot_exchange_info_base_uri, spot_exchange_info_uri);
    std::thread usdFutureThread (usdFutureData, usd_futures_exchange_info_base_uri, usd_futures_exchange_info_uri);
    std::thread coinFutureThread (coinFutureData, coin_futures_exchange_info_base_uri, coin_futures_exchange_info_uri);
 
    spotThread.join();
    usdFutureThread.join();
    coinFutureThread.join();
    return 0;
}

