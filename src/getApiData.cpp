
#include "https.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/writer.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "boost/asio.hpp"
#include "boost/bind/bind.hpp"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "exchangeInfoClass.h"

#include <vector>
#include <map>
#include <thread>
#include <mutex>

// mutex to protect access to shared maps
std::mutex binanceExchangeMutex;

// function to make HTTP request and get data
void fetchData(exchangeInfo& binanceExchange, std::string baseUrl, std::string endpoint) {

    auto const host = baseUrl.c_str();
    auto const port = "443";
    auto const target = endpoint.c_str();
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

    // store retured response in a variable of type http::response<http::string_body>
    http::response<http::string_body> apiResponse;

    // Call to function that retrieves the HTTP response
    apiResponse = returnResponse();
    //std::cout << apiResponse << std::endl;

    // Parse body of HTTP response as JSON
    rapidjson::Document fullData;
    fullData.Parse(apiResponse.body().c_str());

    // Check if parsed data is object and contains symbols array
    if (!fullData.IsObject() || !fullData.HasMember("symbols") || !fullData["symbols"].IsArray()) {
        std::cerr << "Invalid JSON format or missing symbols array." << std::endl;
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
        if(baseUrl == "api.binance.com") { binanceExchange.spotSymbols[info.symbol] = info; }
        if(baseUrl == "dapi.binance.com") { binanceExchange.usdSymbols[info.symbol] = info; }
        if(baseUrl == "fapi.binance.com") { binanceExchange.coinSymbols[info.symbol] = info; } 
   
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

    // Output total number of symbols found
    if(baseUrl == "api.binance.com") { std::cout << "Total SPOT symbols: " << binanceExchange.spotSymbols.size() << std::endl; }
    if(baseUrl == "dapi.binance.com") { std::cout << "Total usd_futures symbols: " << binanceExchange.usdSymbols.size() << std::endl; }
    if(baseUrl == "fapi.binance.com") { std::cout << "Total coin_futures symbols: " << binanceExchange.coinSymbols.size() << std::endl; }
    

}



void query(exchangeInfo& binanceExchange, std::string queryMarket, std::string querySymbol, std::string queryType, std::string queryStatus){

  
    auto it = binanceExchange.spotSymbols.find(querySymbol), it2 = binanceExchange.usdSymbols.find(querySymbol), it3 = binanceExchange.coinSymbols.find(querySymbol);
    if (it == binanceExchange.spotSymbols.end() && it2 == binanceExchange.usdSymbols.end() && it3 == binanceExchange.coinSymbols.end()) {
        std::cout << querySymbol << ": symbol does not exist" << std::endl;
        return;
    } 

    symbolInfo temp;
    if (queryMarket == "SPOT") {
        temp = temp;  
    }
    if (queryMarket == "usd_futures") {
        temp = binanceExchange.usdSymbols[querySymbol];  
    }
    if (queryMarket == "coin_futures") {
        temp = binanceExchange.coinSymbols[querySymbol];  
    }

    rapidjson::Document answers;
    answers.SetObject();
    auto& allocator = answers.GetAllocator();

    if(queryType == "GET"){
        std::cout << "Getting spot data for " << querySymbol << std::endl;

        rapidjson::Value symbolDetails(rapidjson::kObjectType);
        symbolDetails.AddMember("symbol", rapidjson::Value(temp.symbol.c_str(), allocator), allocator);
        symbolDetails.AddMember("quoteAsset", rapidjson::Value(temp.quoteAsset.c_str(), allocator), allocator);
        symbolDetails.AddMember("status", rapidjson::Value(temp.status.c_str(), allocator), allocator);
        symbolDetails.AddMember("tickSize", rapidjson::Value(temp.tickSize.c_str(), allocator), allocator);
        symbolDetails.AddMember("stepSize", rapidjson::Value(temp.stepSize.c_str(), allocator), allocator);

        answers.AddMember("data", symbolDetails, allocator);
        
         
    }

    else if(queryType == "UPDATE"){
        std::cout << "Updating spot data for " << querySymbol << std::endl;
        std::cout << temp.status << std::endl;
        temp.status = queryStatus;
        std::cout << temp.status << std::endl;

        rapidjson::Value updateDetails(rapidjson::kObjectType);
        updateDetails.AddMember("symbol", rapidjson::Value(querySymbol.c_str(), allocator), allocator);
        updateDetails.AddMember("newStatus", rapidjson::Value(queryStatus.c_str(), allocator), allocator);

        answers.AddMember("update", updateDetails, allocator);

    }

    else if(queryType == "DELETE"){
        std::cout << "Deleting spot data for " << querySymbol << std::endl;
        std::cout << temp.symbol << std::endl;
        if(queryMarket == "SPOT") {
            auto it = binanceExchange.spotSymbols.find(querySymbol);
            if (it != binanceExchange.spotSymbols.end()) {
                binanceExchange.spotSymbols.erase(it);
                std::cout << "Deleted symbol " << querySymbol << std::endl;
            } 
        }
        else if(queryMarket == "usd_futures") {
            auto it = binanceExchange.usdSymbols.find(querySymbol);
            if (it != binanceExchange.usdSymbols.end()) {
                binanceExchange.usdSymbols.erase(it);
                std::cout << "Deleted symbol " << querySymbol << std::endl;
            } 
        }
        else if(queryMarket == "coin_futures") {
            auto it = binanceExchange.coinSymbols.find(querySymbol);
            if (it != binanceExchange.coinSymbols.end()) {
                binanceExchange.coinSymbols.erase(it);
                std::cout << "Deleted symbol " << querySymbol << std::endl;
            } 
        }
        else {
            std::cout << "Symbol " << querySymbol << " not found." << std::endl;
        }

        rapidjson::Value deleteDetails(rapidjson::kObjectType);
        deleteDetails.AddMember("deletedSymbol", rapidjson::Value(querySymbol.c_str(), allocator), allocator);

        answers.AddMember("delete", deleteDetails, allocator);
    }
   
    // Write to answers.json file
    FILE* fp2 = fopen("answers.json", "a");
    if (!fp2) {
        std::cerr << "Error: Could not open file for writing." << std::endl;
        return;
    }
    char writeBuffer[65536];
    rapidjson::FileWriteStream os(fp2, writeBuffer, sizeof(writeBuffer));
    rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
    answers.Accept(writer);

    fputs("\n", fp2);
    fclose(fp2);

}

void readQuery(exchangeInfo& binanceExchange) {

    int usdFuturePrevQueryID, spotPrevQueryID, coinFuturePrevQueryID;
    while(true){
        int queryID;
        std::string queryType, queryMarket, querySymbol, queryStatus;

        rapidjson::Document doc;
        std::string queryFile = "query.json";
        FILE* fp = fopen(queryFile.c_str(), "r"); 
        if (!fp) { 
            std::cout << "Error: unable to open file" << std::endl; 
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
                    std::thread queryThread (query, std::ref(binanceExchange), queryMarket, querySymbol, queryType, queryStatus);
                    queryThread.join();
                }
                if(queryMarket == "usd_futures"){
                    std::thread usdFutureQueryThread (query, std::ref(binanceExchange), queryMarket, querySymbol, queryType, queryStatus);
                    usdFutureQueryThread.join();
                }
                if(queryMarket == "coin_futures"){
                    std::thread coinFutureQueryThread (query, std::ref(binanceExchange), queryMarket, querySymbol, queryType, queryStatus);
                    coinFutureQueryThread.join();
                }
            }
            if(queryMarket == "SPOT"){ spotPrevQueryID = queryID; }
            if(queryMarket == "usd_futures"){ usdFuturePrevQueryID = queryID; }
            if(queryMarket == "coin_futures"){ coinFuturePrevQueryID = queryID; }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

