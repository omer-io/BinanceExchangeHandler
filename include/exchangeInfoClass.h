#include <map>
#include <string>
#include <boost/asio/ssl.hpp>

// struct symbolInfo to store required data of symbols
struct symbolInfo{
    std::string symbol, quoteAsset, status, tickSize, stepSize;
};

// class stores symbol info for each endpoint in seperate maps
class exchangeInfo {
private:
    std::map<std::string, symbolInfo> spotSymbols;
    std::map<std::string, symbolInfo> usdSymbols;
    std::map<std::string, symbolInfo> coinSymbols;

public:
    // Getter for spotSymbols
    symbolInfo getSpotSymbol( std::string& key) {
        auto it = spotSymbols.find(key);
            return it->second;
    }

    // Setter for spotSymbols
    void setSpotSymbol(std::string& key, symbolInfo& value) {
        spotSymbols[key] = value;
    }

    // Getter for usdSymbols
    symbolInfo getUsdSymbol(std::string& key){
        auto it = usdSymbols.find(key);
            return it->second;
    }

    // Setter for usdSymbols
    void setUsdSymbol(std::string& key, symbolInfo& value){
        usdSymbols[key] = value;
    }

    // Getter for coinSymbols
    symbolInfo getCoinSymbol(std::string& key) {
        auto it = coinSymbols.find(key);
            return it->second;
    }

    // Setter for coinSymbols
    void setCoinSymbol( std::string& key, symbolInfo& value) {
        coinSymbols[key] = value;
    }

    // Function to get the size of spotSymbols
    size_t getSpotSymbolsSize(){
        return spotSymbols.size();
    }

    // Function to get the size of usdSymbols
    size_t getUsdSymbolsSize(){
        return usdSymbols.size();
    }

    // Function to get the size of coinSymbols
    size_t getCoinSymbolsSize(){
        return coinSymbols.size();
    }

    void updateSpotStatus(std::string key, std::string newStatus){
        spotSymbols[key].status = newStatus;
    }
    void updateUsdStatus(std::string key, std::string newStatus){
        usdSymbols[key].status = newStatus;
    }
    void updateCoinStatus(std::string key, std::string newStatus){
        coinSymbols[key].status = newStatus;
    }

    void deleteSpotSymbol(std::string key){
        auto it = spotSymbols.find(key);
        spotSymbols.erase(it);
    }
    void deleteUsdSymbol(std::string key){
        auto it = usdSymbols.find(key);
        usdSymbols.erase(key);
    }
    void deleteCoinSymbol(std::string key){
        auto it = coinSymbols.find(key);
        coinSymbols.erase(key);
    }

        // check if spot symbol exists
    bool spotSymbolexists(std::string& key){
        auto it = spotSymbols.find(key);
        if (it != spotSymbols.end()) {
            return true;
        }
        return false;
    }

    // check if usd symbol exists
    bool usdSymbolexists(std::string& key){
        auto it = spotSymbols.find(key);
        if (it != spotSymbols.end()) {
            return true;
        }
        return false;
    }

    // check if coin symbol exists
    bool coinSymbolexists(std::string& key){
        auto it = spotSymbols.find(key);
        if (it != spotSymbols.end()) {
            return true;
        }
        return false;
    }
};


// functions declared here are defined in src/getApiData.cpp
void fetchData(exchangeInfo&, std::string&, std::string&, boost::asio::io_context&, boost::asio::ssl::context&);
void readQuery(exchangeInfo&);
void query(exchangeInfo&, std::string&, std::string&, std::string&, std::string&);
void setLogLevelForBM();