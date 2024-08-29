
#include <vector>
#include <map>
#include <thread>
#include <mutex>

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

// mutex to protect access to shared maps
std::mutex binanceExchangeMutex;

// function to make HTTP request and get data
void fetchData(exchangeInfo& binanceExchange, std::string& baseUrl, std::string& endpoint, boost::asio::io_context& ioc, boost::asio::ssl::context& ctx) {

    auto const host = baseUrl.c_str();
    auto const port = "443";
    auto const target = endpoint.c_str();
    int version = 11;

    spdlog::info("Starting async HTTP request to host: {}, endpoint: {}", host, target);
    // Launch the asynchronous operation
    // The session is constructed with a strand to
    // ensure that handlers do not execute concurrently.
    std::make_shared<session>(
        net::make_strand(ioc),
        ctx, binanceExchange, baseUrl
        )->run(host, port, target, version);

}


// function to perform queries
void query(exchangeInfo& binanceExchange, std::string& queryMarket, std::string& querySymbol, std::string& queryType, std::string& queryStatus){

    spdlog::info("Processing query: Market = {}, Symbol = {}, Type = {}", queryMarket, querySymbol, queryType);

    // Check if the symbol exists in any of the markets 
    if(queryMarket == "SPOT"){
        if(!binanceExchange.spotSymbolexists(querySymbol)){
            spdlog::error("{}: symbol does not exist", querySymbol);
            return;   
        }
    }
    if(queryMarket == "usd_futures"){
        if(!binanceExchange.usdSymbolexists(querySymbol)){
            spdlog::error("{}: symbol does not exist", querySymbol);
            return;   
        }
    }
        if(queryMarket == "coin_futures"){
        if(!binanceExchange.coinSymbolexists(querySymbol)){
            spdlog::error("{}: symbol does not exist", querySymbol);
            return;   
        }
    }

    // Retrieve symbol info based on specified market asreference to access original symbol data
    symbolInfo temp;
    if (queryMarket == "SPOT") { temp = binanceExchange.getSpotSymbol(querySymbol); }
    if (queryMarket == "usd_futures") { temp = binanceExchange.getUsdSymbol(querySymbol); }
    if (queryMarket == "coin_futures") { temp = binanceExchange.getCoinSymbol(querySymbol); }
    
    // Create a RapidJSON document to store the results
    rapidjson::Document answers;
    answers.SetObject();
    auto& allocator = answers.GetAllocator();
    
    // lock while performing query on data structure
    binanceExchangeMutex.lock();

    // Process query based on type
    if(queryType == "GET"){

        // GET request: retrieve and output symbol to answers.json
        spdlog::info("Getting spot data for {}", querySymbol);

        rapidjson::Value symbolDetails(rapidjson::kObjectType);
        symbolDetails.AddMember("symbol", rapidjson::Value(temp.symbol.c_str(), allocator), allocator);
        symbolDetails.AddMember("quoteAsset", rapidjson::Value(temp.quoteAsset.c_str(), allocator), allocator);
        symbolDetails.AddMember("status", rapidjson::Value(temp.status.c_str(), allocator), allocator);
        symbolDetails.AddMember("tickSize", rapidjson::Value(temp.tickSize.c_str(), allocator), allocator);
        symbolDetails.AddMember("stepSize", rapidjson::Value(temp.stepSize.c_str(), allocator), allocator);

        answers.AddMember("data", symbolDetails, allocator);
        
    }

    else if(queryType == "UPDATE"){

        // UPDATE request: modify symbol status and output update details to answers.json
        spdlog::info("Updating data for symbol: {}", querySymbol);

        if(queryMarket == "SPOT"){
            spdlog::info("Old Status: {}", temp.status);
            binanceExchange.updateSpotStatus(querySymbol, queryStatus);
            temp = binanceExchange.getSpotSymbol(querySymbol);
            spdlog::info("New Status: {}", temp.status);

            rapidjson::Value updateDetails(rapidjson::kObjectType);
            updateDetails.AddMember("symbol", rapidjson::Value(querySymbol.c_str(), allocator), allocator);
            updateDetails.AddMember("newStatus", rapidjson::Value(queryStatus.c_str(), allocator), allocator);
            answers.AddMember("update", updateDetails, allocator);
        }

        if(queryMarket == "usd_futures"){
            spdlog::info("Old Status: {}", temp.status);
            binanceExchange.updateUsdStatus(querySymbol, queryStatus);
            temp = binanceExchange.getUsdSymbol(querySymbol);
            spdlog::info("New Status: {}", temp.status);

            rapidjson::Value updateDetails(rapidjson::kObjectType);
            updateDetails.AddMember("symbol", rapidjson::Value(querySymbol.c_str(), allocator), allocator);
            updateDetails.AddMember("newStatus", rapidjson::Value(queryStatus.c_str(), allocator), allocator);
            answers.AddMember("update", updateDetails, allocator);
        }

        if(queryMarket == "coin_futures"){
            spdlog::info("Old Status: {}", temp.status);
            binanceExchange.updateCoinStatus(querySymbol, queryStatus);
            temp = binanceExchange.getCoinSymbol(querySymbol);
            spdlog::info("New Status: {}", temp.status);

            rapidjson::Value updateDetails(rapidjson::kObjectType);
            updateDetails.AddMember("symbol", rapidjson::Value(querySymbol.c_str(), allocator), allocator);
            updateDetails.AddMember("newStatus", rapidjson::Value(queryStatus.c_str(), allocator), allocator);
            answers.AddMember("update", updateDetails, allocator);
        }

    }

    else if(queryType == "DELETE"){

        // DELETE request: remove symbol from respective market and output delete status to answers.json
        spdlog::info("Deleting data for symbol: {}", querySymbol);
        rapidjson::Value deleteDetails(rapidjson::kObjectType);
        if(queryMarket == "SPOT") {
            binanceExchange.deleteSpotSymbol(querySymbol);
            spdlog::info("Deleted symbol {}", querySymbol);
            deleteDetails.AddMember("deletedSymbol", rapidjson::Value(querySymbol.c_str(), allocator), allocator);
            answers.AddMember("delete", deleteDetails, allocator);
        } 

        else if(queryMarket == "usd_futures") {
            binanceExchange.deleteUsdSymbol(querySymbol);
            spdlog::info("Deleted symbol {}", querySymbol);
            deleteDetails.AddMember("deletedSymbol", rapidjson::Value(querySymbol.c_str(), allocator), allocator);
            answers.AddMember("delete", deleteDetails, allocator);
        }
        else if(queryMarket == "coin_futures") {
            binanceExchange.deleteCoinSymbol(querySymbol);
            spdlog::info("Deleted symbol {}", querySymbol);
            deleteDetails.AddMember("deletedSymbol", rapidjson::Value(querySymbol.c_str(), allocator), allocator);
            answers.AddMember("delete", deleteDetails, allocator);
        }
        else {
            spdlog::warn("Symbol {} not found for deletion.", querySymbol);
        }

    }
    // unlock after performing query    
    binanceExchangeMutex.unlock();
    // Write to answers.json file
    FILE* fp2 = fopen("answers.json", "a");
    if (!fp2) {
        spdlog::error("Could not open answers.json file for writing.");
        return;
    }
    char writeBuffer[65536];
    rapidjson::FileWriteStream os(fp2, writeBuffer, sizeof(writeBuffer));
    rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
    answers.Accept(writer);

    fputs("\n", fp2);
    fclose(fp2);

}

// function to continuously read and process queries from query.JSON file
void readQuery(exchangeInfo& binanceExchange) {

    // variables to keep track of last query ID for each market type
    std::vector<long long int> prevIDs;

    // infinite loop to continuously process queries
    while(true){

        // Variables to store query details
        long long int queryID;
        std::string queryType, queryMarket, querySymbol, queryStatus;

        // Load and parse the JSON query file
        rapidjson::Document doc;
        std::string queryFile = "query.json";
        FILE* fp = fopen(queryFile.c_str(), "r"); 
        if (!fp) { 
            spdlog::error("Error: unable to open file {}", queryFile);
        } 

        char buffer[65536];
        rapidjson::FileReadStream is(fp, buffer, sizeof(buffer));
        doc.ParseStream(is);
        fclose(fp); 

        // Process each query 
        for (rapidjson::Value::ConstValueIterator itr = doc["query"].Begin(); itr != doc["query"].End(); ++itr) {
            // Extract query details
            int queryID = (*itr)["id"].GetInt();
            std::string queryType = (*itr)["query_type"].GetString();
            std::string queryMarket = (*itr)["market_type"].GetString();
            std::string querySymbol = (*itr)["instrument_name"].GetString();

            // Check if the query has a status field
            if (itr->HasMember("data")) {
                if ((*itr)["data"].HasMember("status")) {
                    queryStatus = (*itr)["data"]["status"].GetString();
                    // std::cout << "Status: " << queryStatus << std::endl;
                }
            }
            bool idFlag = true;
            // execute query if it has not been processed before for the same market
            for(int index = 0; index < prevIDs.size(); ++index){
                if(queryID == prevIDs[index]){
                    idFlag = false;
                    break;
                }
            }
            if(idFlag == true){
                query(binanceExchange, queryMarket, querySymbol, queryType, queryStatus);
            }
            // Update the last processed query ID
            prevIDs.push_back(queryID);
        }
        // Sleep for 1 sec before reading the file again
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void setLogLevelForBM(){
        spdlog::set_level(spdlog::level::off);
}
