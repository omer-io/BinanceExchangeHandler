
#include "https.h"
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include <vector>
#include <map>
#include <thread>
struct symbolInfo{
    std::string symbol;
    std::string quoteAsset;
    std::string status;
    std::string tickSize;
    std::string stepSize;
};

class exchangeInfo{
    public:
        std::map<std::string, symbolInfo> spotSymbols;
        std::map<std::string, symbolInfo> usdSymbols;
        std::map<std::string, symbolInfo> coinSymbols;
};

exchangeInfo binanceExchange;

void spotData(std::string spotBaseUrl, std::string spotEndpoint) {
    auto const host = spotBaseUrl.c_str();
    auto const port = "443";
    auto const target = spotEndpoint.c_str();
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
    //std::cout << spotExchange << std::endl;

    rapidjson::Document fullData;
    fullData.Parse(spotExchange.body().c_str());

    if (!fullData.IsObject() || !fullData.HasMember("symbols") || !fullData["symbols"].IsArray()) {
        std::cerr << "Invalid JSON format or missing symbols array." << std::endl;
    }

    const auto& symbolsArray = fullData["symbols"];

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

        binanceExchange.spotSymbols[info.symbol] = info;
    }

    // for (const auto& pair : binanceExchange.spotSymbols) {
    //     const auto& symbolInfo = pair.second; 

    //     std::cout << "Symbol: " << symbolInfo.symbol << std::endl;
    //     std::cout << "Quote Asset: " << symbolInfo.quoteAsset << std::endl;
    //     std::cout << "Status: " << symbolInfo.status << std::endl;
    //     std::cout << "Tick Size: " << symbolInfo.tickSize << std::endl;
    //     std::cout << "Step Size: " << symbolInfo.stepSize << std::endl;
    //     std::cout << "---------------------" << std::endl;
    // }

    std::cout << "Total SPOT symbols: " << binanceExchange.spotSymbols.size() << std::endl;

}

void usdFutureData(std::string usdFutureBaseUrl, std::string usdFutureEndpoint) {
    auto const host = usdFutureBaseUrl.c_str();
    auto const port = "443";
    auto const target = usdFutureEndpoint.c_str();
    int version = 11;

    net::io_context ioc;

    ssl::context ctx{ssl::context::tlsv12_client};

    load_root_certificates(ctx);

    ctx.set_verify_mode(ssl::verify_peer);

    std::make_shared<session>(
        net::make_strand(ioc),
        ctx
        )->run(host, port, target, version);

    ioc.run();

    http::response<http::string_body> usdFutureExchange;
    usdFutureExchange = returnResponse();
    //std::cout << usdFutureExchange << std::endl;

    rapidjson::Document fullData;
    fullData.Parse(usdFutureExchange.body().c_str());

    if (!fullData.IsObject() || !fullData.HasMember("symbols") || !fullData["symbols"].IsArray()) {
        std::cerr << "Invalid JSON format or missing symbols array." << std::endl;
    }

    const auto& symbolsArray = fullData["symbols"];

    for (const auto& symbol : symbolsArray.GetArray()) {
        symbolInfo info;
        info.symbol = symbol["symbol"].GetString();
        info.quoteAsset = symbol["quoteAsset"].GetString();
        info.status = symbol["contractStatus"].GetString();

        for (const auto& filter : symbol["filters"].GetArray()) {
            std::string filterType = filter["filterType"].GetString();
            if (filterType == "PRICE_FILTER") {
                info.tickSize = filter["tickSize"].GetString();
            } else if (filterType == "LOT_SIZE") {
                info.stepSize = filter["stepSize"].GetString();
            }
        }

        binanceExchange.usdSymbols[info.symbol] = info;
    }

    // for (const auto& pair : binanceExchange.usdSymbols) {
    //     const auto& symbolInfo = pair.second; 

    //     std::cout << "Symbol: " << symbolInfo.symbol << std::endl;
    //     std::cout << "Quote Asset: " << symbolInfo.quoteAsset << std::endl;
    //     std::cout << "Status: " << symbolInfo.status << std::endl;
    //     std::cout << "Tick Size: " << symbolInfo.tickSize << std::endl;
    //     std::cout << "Step Size: " << symbolInfo.stepSize << std::endl;
    //     std::cout << "---------------------" << std::endl;
    // }

    std::cout << "Total USD Futures symbols: " << binanceExchange.usdSymbols.size() << std::endl;


}

void coinFutureData(std::string coinFutureBaseUrl, std::string coinFutureEndpoint){
        auto const host = coinFutureBaseUrl.c_str();
    auto const port = "443";
    auto const target = coinFutureEndpoint.c_str();
    int version = 11;

    net::io_context ioc;

    ssl::context ctx{ssl::context::tlsv12_client};

    load_root_certificates(ctx);

    ctx.set_verify_mode(ssl::verify_peer);

    std::make_shared<session>(
        net::make_strand(ioc),
        ctx
        )->run(host, port, target, version);

    ioc.run();

    http::response<http::string_body> coinFutureExchange;
    coinFutureExchange = returnResponse();
    // std::cout << coinFutureExchange << std::endl;

    rapidjson::Document fullData;
    fullData.Parse(coinFutureExchange.body().c_str());

    if (!fullData.IsObject() || !fullData.HasMember("symbols") || !fullData["symbols"].IsArray()) {
        std::cerr << "Invalid JSON format or missing symbols array." << std::endl;
    }

    const auto& symbolsArray = fullData["symbols"];

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

        binanceExchange.coinSymbols[info.symbol] = info;
    }

    // for (const auto& pair : binanceExchange.coinSymbols) {
    //     const auto& symbolInfo = pair.second; 

    //     std::cout << "Symbol: " << symbolInfo.symbol << std::endl;
    //     std::cout << "Quote Asset: " << symbolInfo.quoteAsset << std::endl;
    //     std::cout << "Status: " << symbolInfo.status << std::endl;
    //     std::cout << "Tick Size: " << symbolInfo.tickSize << std::endl;
    //     std::cout << "Step Size: " << symbolInfo.stepSize << std::endl;
    //     std::cout << "---------------------" << std::endl;
    // }

    std::cout << "Total Coin Futures symbols: " << binanceExchange.coinSymbols.size() << std::endl;

}

void spotQuery(std::string spotQuerySymbol, std::string spotQueryType, std::string spotQueryStatus){
            
    auto it = binanceExchange.spotSymbols.find(spotQuerySymbol);
    if (it == binanceExchange.spotSymbols.end()) {
        std::cout << spotQuerySymbol << ": symbol does not exist" << std::endl;
        return;
    } 
    if(spotQueryType == "GET"){
        std::cout << "Getting spot data for " << spotQuerySymbol << std::endl;
        symbolInfo temp = binanceExchange.spotSymbols[spotQuerySymbol];
        std::cout << "Symbol: " << temp.symbol << std::endl;
        std::cout << "Quote Asset: " << temp.quoteAsset << std::endl;
        std::cout << "Status: " << temp.status << std::endl;
        std::cout << "Tick Size: " << temp.tickSize << std::endl;
        std::cout << "Step Size: " << temp.stepSize << std::endl;
        std::cout << "---------------------" << std::endl;
    }

    else if(spotQueryType == "UPDATE"){
        std::cout << "Updating spot data for " << spotQuerySymbol << std::endl;
        std::cout << binanceExchange.spotSymbols[spotQuerySymbol].status << std::endl;
        binanceExchange.spotSymbols[spotQuerySymbol].status = spotQueryStatus;
        std::cout << binanceExchange.spotSymbols[spotQuerySymbol].status << std::endl;

    }

    else if(spotQueryType == "DELETE"){
        std::cout << "Deleting spot data for " << spotQuerySymbol << std::endl;
        std::cout << binanceExchange.spotSymbols[spotQuerySymbol].symbol << std::endl;
        auto it = binanceExchange.spotSymbols.find(spotQuerySymbol);
        if (it != binanceExchange.spotSymbols.end()) {
            binanceExchange.spotSymbols.erase(it);
            std::cout << "Deleted symbol " << spotQuerySymbol << std::endl;
        } 
        else {
            std::cout << "Symbol " << spotQuerySymbol << " not found." << std::endl;
        }
       // std::cout << "deletion" << binanceExchange.spotSymbols[spotQuerySymbol].symbol << std::endl;
    }
}

void readQuery() {

    int usdFuturePrevQueryID, spotPrevQueryID, coinFuturePrevQueryID;
    while(true){
        int queryID;
        std::string queryType, queryMarket, querySymbol, queryStatus;

        rapidjson::Document doc;
        std::string queryFile = "/home/omer/training/BinanceExchangeHandler/query.json";
        FILE* fp = fopen(queryFile.c_str(), "r"); 
        if (!fp) { 
            std::cerr << "Error: unable to open file" << std::endl; 
        } 

        char buffer[65536];
        rapidjson::FileReadStream is(fp, buffer, sizeof(buffer));

        doc.ParseStream(is);
        fclose(fp); 

        for (rapidjson::Value::ConstValueIterator itr = doc["query"].Begin(); itr != doc["query"].End(); ++itr) {
            int queryID = (*itr)["id"].GetInt();
            std::string queryType = (*itr)["query_type"].GetString();
            std::string queryMarket = (*itr)["market_type"].GetString();
            std::string querySymbol = (*itr)["instrument_name"].GetString();

            // std::cout << "Query ID: " << queryID << std::endl;
            // std::cout << "Query Type: " << queryType << std::endl;
            // std::cout << "Market Type: " << queryMarket << std::endl;
            // std::cout << "Instrument Name: " << querySymbol << std::endl;

            if (itr->HasMember("data")) {
                if ((*itr)["data"].HasMember("status")) {
                    queryStatus = (*itr)["data"]["status"].GetString();
                    // std::cout << "Status: " << queryStatus << std::endl;
                }
            }
            if(queryID != spotPrevQueryID && queryID != usdFuturePrevQueryID && queryID != coinFuturePrevQueryID){
                if(queryMarket == "SPOT"){
                    std::thread spotQueryThread (spotQuery, querySymbol, queryType, queryStatus);
                    spotQueryThread.join();
                }
                if(queryMarket == "usd_futures"){
                    std::thread usdFutureQueryThread (spotQuery, querySymbol, queryType, queryStatus);
                    usdFutureQueryThread.join();
                }
                if(queryMarket == "coin_futures"){
                    std::thread coinFutureQueryThread (spotQuery, querySymbol, queryType, queryStatus);
                    coinFutureQueryThread.join();
                }
            }
            if(queryMarket == "SPOT"){
                spotPrevQueryID = queryID;
            }
            if(queryMarket == "usd_futures"){
                usdFuturePrevQueryID = queryID;
            }
            if(queryMarket == "coin_futures"){
                coinFuturePrevQueryID = queryID;
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

