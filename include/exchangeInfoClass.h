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
    symbolInfo getSpotSymbol(const std::string& key) const {
        auto it = spotSymbols.find(key);
        if (it != spotSymbols.end()) {
            return it->second;
        }
        // Return a default symbolInfo object if the key is not found
        return symbolInfo{};
    }

    // Setter for spotSymbols
    void setSpotSymbol(const std::string& key, const symbolInfo& value) {
        spotSymbols[key] = value;
    }

    // Getter for usdSymbols
    symbolInfo getUsdSymbol(const std::string& key) const {
        auto it = usdSymbols.find(key);
        if (it != usdSymbols.end()) {
            return it->second;
        }
        // Return a default symbolInfo object if the key is not found
        return symbolInfo{};
    }

    // Setter for usdSymbols
    void setUsdSymbol(const std::string& key, const symbolInfo& value) {
        usdSymbols[key] = value;
    }

    // Getter for coinSymbols
    symbolInfo getCoinSymbol(const std::string& key) const {
        auto it = coinSymbols.find(key);
        if (it != coinSymbols.end()) {
            return it->second;
        }
        // Return a default symbolInfo object if the key is not found
        return symbolInfo{};
    }

    // Setter for coinSymbols
    void setCoinSymbol(const std::string& key, const symbolInfo& value) {
        coinSymbols[key] = value;
    }
};


// functions declared here are defined in src/getApiData.cpp
void fetchData(exchangeInfo&, std::string, std::string, boost::asio::io_context&, boost::asio::ssl::context&);
void readQuery(exchangeInfo&);
void query(exchangeInfo&, std::string, std::string, std::string, std::string);
void setLogLevelForBM();