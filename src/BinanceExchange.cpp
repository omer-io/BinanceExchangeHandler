
#include <vector>
#include <mutex>

#include "getHttpsData.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/writer.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

// mutex to protect access to shared maps
std::mutex binanceExchangeMutex;

// Getter for spotSymbols
symbolInfo exchangeInfo::getSpotSymbol( std::string& key) {
    auto it = spotSymbols.find(key);
        return it->second;
}

// Setter for spotSymbols
void exchangeInfo::setSpotSymbol(std::string& key, symbolInfo& value) {
    spotSymbols[key] = value;
}

// Getter for usdSymbols
symbolInfo exchangeInfo::getUsdSymbol(std::string& key){
    auto it = usdSymbols.find(key);
        return it->second;
}

// Setter for usdSymbols
void exchangeInfo::setUsdSymbol(std::string& key, symbolInfo& value){
    usdSymbols[key] = value;
}

// Getter for coinSymbols
symbolInfo exchangeInfo::getCoinSymbol(std::string& key) {
    auto it = coinSymbols.find(key);
        return it->second;
}

// Setter for coinSymbols
void exchangeInfo::setCoinSymbol( std::string& key, symbolInfo& value) {
    coinSymbols[key] = value;
}

// Function to get the size of spotSymbols
size_t exchangeInfo::getSpotSymbolsSize(){
    return spotSymbols.size();
}

// Function to get the size of usdSymbols
size_t exchangeInfo::getUsdSymbolsSize(){
    return usdSymbols.size();
}

// Function to get the size of coinSymbols
size_t exchangeInfo::getCoinSymbolsSize(){
    return coinSymbols.size();
}

void exchangeInfo::updateSpotStatus(std::string& key, std::string& newStatus){
    spotSymbols[key].status = newStatus;
}
void exchangeInfo::updateUsdStatus(std::string& key, std::string& newStatus){
    usdSymbols[key].status = newStatus;
}
void exchangeInfo::updateCoinStatus(std::string& key, std::string& newStatus){
    coinSymbols[key].status = newStatus;
}

void exchangeInfo::deleteSpotSymbol(std::string& key){
    auto it = spotSymbols.find(key);
    spotSymbols.erase(it);
}
void exchangeInfo::deleteUsdSymbol(std::string& key){
    auto it = usdSymbols.find(key);
    usdSymbols.erase(key);
}
void exchangeInfo::deleteCoinSymbol(std::string& key){
    auto it = coinSymbols.find(key);
    coinSymbols.erase(key);
}

    // check if spot symbol exists
bool exchangeInfo::spotSymbolexists(std::string& key){
    auto it = spotSymbols.find(key);
    if (it != spotSymbols.end()) {
        return true;
    }
    return false;
}

// check if usd symbol exists
bool exchangeInfo::usdSymbolexists(std::string& key){
    auto it = usdSymbols.find(key);
    if (it != usdSymbols.end()) {
        return true;
    }
    return false;
}

// check if coin symbol exists
bool exchangeInfo::coinSymbolexists(std::string& key){
    auto it = coinSymbols.find(key);
    if (it != coinSymbols.end()) {
        return true;
    }
    return false;
}

// read config.json for logging, request url, request interval
void exchangeInfo::readConfig(std::string configFile, urlInfo& urlConfig, logsInfo& logsConfig) {

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
    urlConfig.spotExchangeBaseUrl = doc["exchange_base_url"]["spot_exchange_info_base_uri"].GetString();
    urlConfig.usdFutureExchangeBaseUrl = doc["exchange_base_url"]["usd_futures_exchange_info_base_uri"].GetString();
    urlConfig.coinFutureExchangeBaseUrl = doc["exchange_base_url"]["coin_futures_exchange_info_base_uri"].GetString();

    // store each endpoint
    urlConfig.spotExchangeEndpoint = doc["exchange_endpoints"]["spot_exchange_info_uri"].GetString();
    urlConfig.usdFutureEndpoint = doc["exchange_endpoints"]["usd_futures_exchange_info_uri"].GetString();
    urlConfig.coinFutureEndpoint = doc["exchange_endpoints"]["coin_futures_exchange_info_uri"].GetString();
    urlConfig.requestInterval = doc["request_interval"].GetInt();
    
    // store logging level, file enable, console enable
    logsConfig.level = doc["logging"]["level"].GetString();
    logsConfig.file = doc["logging"]["file"].GetBool();
    logsConfig.console = doc["logging"]["console"].GetBool();

    // close file
    fclose(fp); 

    spdlog::info("Config file loaded successfully");
}

void exchangeInfo::setSpdLogs(logsInfo& logsConfig){

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

// function to make HTTP request and get data
void exchangeInfo::fetchData(urlInfo& urlConfig, boost::asio::io_context& ioc, boost::asio::ssl::context& ctx) {

    auto const port = "443";
    int version = 11;

    // Launch the asynchronous operation
    // The session is constructed with a strand to ensure that handlers do not execute concurrently.

    // creating session for spot endpoint
    spdlog::info("Starting async HTTP request to host: {}, endpoint: {}", urlConfig.spotExchangeBaseUrl, urlConfig.spotExchangeEndpoint);
    auto spotSession = std::make_shared<session>(boost::asio::make_strand(ioc), ctx, this, urlConfig);
    spotSession->get(urlConfig.spotExchangeBaseUrl.c_str(), port, urlConfig.spotExchangeEndpoint.c_str(), version);

    // creating session for usd futures endpoint
    spdlog::info("Starting async HTTP request to host: {}, endpoint: {}", urlConfig.usdFutureExchangeBaseUrl, urlConfig.usdFutureEndpoint);
    auto usdFuturesSession = std::make_shared<session>(boost::asio::make_strand(ioc), ctx, this, urlConfig);
    usdFuturesSession->get(urlConfig.usdFutureExchangeBaseUrl.c_str(), port, urlConfig.usdFutureEndpoint.c_str(), version);

    // creating session for coin futures endpoint
    spdlog::info("Starting async HTTP request to host: {}, endpoint: {}", urlConfig.coinFutureExchangeBaseUrl, urlConfig.coinFutureEndpoint);
    auto coinFuturesSession = std::make_shared<session>(boost::asio::make_strand(ioc), ctx, this, urlConfig);
    coinFuturesSession->get(urlConfig.coinFutureExchangeBaseUrl.c_str(), port, urlConfig.coinFutureEndpoint.c_str(), version);

}

// function to perform queries
void exchangeInfo::query(std::string& queryMarket, std::string& querySymbol, std::string& queryType, std::string& queryStatus){

    spdlog::info("Processing query: Market = {}, Symbol = {}, Type = {}", queryMarket, querySymbol, queryType);

    // Check if the symbol exists in any of the markets 
    if(queryMarket == "SPOT"){
        if(!spotSymbolexists(querySymbol)){
            spdlog::error("{}: symbol does not exist", querySymbol);
            return;   
        }
    }
    if(queryMarket == "usd_futures"){
        if(!usdSymbolexists(querySymbol)){
            spdlog::error("{}: symbol does not exist", querySymbol);
            return;   
        }
    }
    if(queryMarket == "coin_futures"){
        if(!coinSymbolexists(querySymbol)){
            spdlog::error("{}: symbol does not exist", querySymbol);
            return;   
        }
    }

    // Retrieve symbol info based on specified market asreference to access original symbol data
    symbolInfo temp;
    if (queryMarket == "SPOT") { temp = getSpotSymbol(querySymbol); }
    if (queryMarket == "usd_futures") { temp = getUsdSymbol(querySymbol); }
    if (queryMarket == "coin_futures") { temp = getCoinSymbol(querySymbol); }
    
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
            spdlog::info("Old Status: {}", getSpotSymbol(querySymbol).status);
            updateSpotStatus(querySymbol, queryStatus);
            spdlog::info("New Status: {}", getSpotSymbol(querySymbol).status);

            rapidjson::Value updateDetails(rapidjson::kObjectType);
            updateDetails.AddMember("symbol", rapidjson::Value(querySymbol.c_str(), allocator), allocator);
            updateDetails.AddMember("newStatus", rapidjson::Value(queryStatus.c_str(), allocator), allocator);
            answers.AddMember("update", updateDetails, allocator);
        }

        if(queryMarket == "usd_futures"){
            spdlog::info("Old Status: {}", getUsdSymbol(querySymbol).status);
            updateUsdStatus(querySymbol, queryStatus);
            spdlog::info("New Status: {}", getUsdSymbol(querySymbol).status);

            rapidjson::Value updateDetails(rapidjson::kObjectType);
            updateDetails.AddMember("symbol", rapidjson::Value(querySymbol.c_str(), allocator), allocator);
            updateDetails.AddMember("newStatus", rapidjson::Value(queryStatus.c_str(), allocator), allocator);
            answers.AddMember("update", updateDetails, allocator);
        }

        if(queryMarket == "coin_futures"){
            spdlog::info("Old Status: {}", getCoinSymbol(querySymbol).status);
            updateCoinStatus(querySymbol, queryStatus);
            spdlog::info("New Status: {}", getCoinSymbol(querySymbol).status);

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
            deleteSpotSymbol(querySymbol);
            spdlog::info("Deleted symbol {}", querySymbol);
            deleteDetails.AddMember("deletedSymbol", rapidjson::Value(querySymbol.c_str(), allocator), allocator);
            answers.AddMember("delete", deleteDetails, allocator);
        } 

        else if(queryMarket == "usd_futures") {
            deleteUsdSymbol(querySymbol);
            spdlog::info("Deleted symbol {}", querySymbol);
            deleteDetails.AddMember("deletedSymbol", rapidjson::Value(querySymbol.c_str(), allocator), allocator);
            answers.AddMember("delete", deleteDetails, allocator);
        }
        else if(queryMarket == "coin_futures") {
            deleteCoinSymbol(querySymbol);
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
void exchangeInfo::readQuery() {

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
                this->query(queryMarket, querySymbol, queryType, queryStatus);
            }
            // Update the last processed query ID
            prevIDs.push_back(queryID);
        }
        // Sleep for 1 sec before reading the file again
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
