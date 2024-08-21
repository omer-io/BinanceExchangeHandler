#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

#include "https.h"

// global variables
std::string spot_exchange_info_uri;
std::string usd_futures_exchange_info_uri;
std::string coin_futures_exchange_info_uri;

std::string spot_exchange_info_base_uri;
std::string usd_futures_exchange_info_base_uri;
std::string coin_futures_exchange_info_base_uri;
int request_interval;

rapidjson::Document doc;
void readConfig(std::string configFile) {

    FILE* fp = fopen(configFile.c_str(), "r"); 
    if (!fp) {
        throw std::runtime_error("Could not open config file: " + configFile);
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
struct symbolInfo{
    std::string symbol;
    std::string quoteAsset;
    std::string status;
    std::string tickSize;
    std::string stepSize;
};

class exchangeInfo{
    private:
        std::vector<symbolInfo> symbols;
};

int main() {

    readConfig("/home/omer/training/BinanceExchangeHandler/config.json");

    std::cout << "Spot Exchange Info base URI: " << spot_exchange_info_base_uri << std::endl;
    std::cout << "USD Futures Exchange Info base URI: " << usd_futures_exchange_info_base_uri << std::endl;
    std::cout << "Coin Futures Exchange Info base URI: " << coin_futures_exchange_info_base_uri << std::endl;
    std::cout << "Spot Exchange Info URI: " << spot_exchange_info_uri << std::endl;
    std::cout << "USD Futures Exchange Info URI: " << usd_futures_exchange_info_uri << std::endl;
    std::cout << "Coin Futures Exchange Info URI: " << coin_futures_exchange_info_uri << std::endl;
    std::cout << "Request Interval: " << request_interval << " seconds" << std::endl;


    auto const host = doc["exchange_base_url"]["spot_exchange_info_base_uri"].GetString();
    auto const port = "443";
    auto const target = doc["exchange_endpoints"]["spot_exchange_info_uri"].GetString();
    int version = 11;
    
    // The io_context is required for all I/O
    net::io_context ioc;

    // The SSL context is required, and holds certificates
    ssl::context ctx{ssl::context::tlsv12_client};

    // This holds the root certificate used for verification
    load_root_certificates(ctx);

    // Verify the remote server's certificate
    ctx.set_verify_mode(ssl::verify_peer);

    // Launch the asynchronous operation
    // The session is constructed with a strand to
    // ensure that handlers do not execute concurrently.
    std::make_shared<session>(
        net::make_strand(ioc),
        ctx
        )->run(host, port, target, version);

    // Run the I/O service. The call will return when
    // the get operation is complete.
    ioc.run();

    http::response<http::string_body> spotExchange;
    spotExchange = returnResponse();
    std::cout << spotExchange << std::endl;

    rapidjson::Document fullData;
    fullData.Parse(spotExchange.body().c_str());

    if (!fullData.IsObject() || !fullData.HasMember("symbols") || !fullData["symbols"].IsArray()) {
        std::cerr << "Invalid JSON format or missing symbols array." << std::endl;
        return 0;
    }

    const auto& symbolsArray = fullData["symbols"];
    std::vector<symbolInfo> symbols;

    for (const auto& symbol : symbolsArray.GetArray()) {
        symbolInfo info;
        info.symbol = symbol["symbol"].GetString();
        info.quoteAsset = symbol["quoteAsset"].GetString();
        info.status = symbol["status"].GetString();

        for (const auto& filter : symbol["filters"].GetArray()) {
            std::string filterType = filter["filterType"].GetString();
            if (filterType == "PRICE_FILTER") {
                info.tickSize = filter["tickSize"].GetString();
            } else if (filterType == "LOT_SIZE") {
                info.stepSize = filter["stepSize"].GetString();
            }
        }

        symbols.push_back(info);
    }

    for (const auto& symbolInfo : symbols) {
        std::cout << "Symbol: " << symbolInfo.symbol << std::endl;
        std::cout << "Quote Asset: " << symbolInfo.quoteAsset << std::endl;
        std::cout << "Status: " << symbolInfo.status << std::endl;
        std::cout << "Tick Size: " << symbolInfo.tickSize << std::endl;
        std::cout << "Step Size: " << symbolInfo.stepSize << std::endl;
        std::cout << "---------------------" << std::endl;
    }
    std::cout << symbols.size() << std::endl; 
    return 0;
}

