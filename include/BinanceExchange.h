#ifndef BinanceExchange_H
#define BinanceExchange_H

#include <map>
#include <string>
#include <boost/asio/ssl.hpp>

// struct to store base url and endpoints info
struct urlInfo{
    std::string spotExchangeBaseUrl; 
    std::string usdFutureExchangeBaseUrl;
    std::string coinFutureExchangeBaseUrl;
    std::string spotExchangeEndpoint;
    std::string usdFutureEndpoint;
    std::string coinFutureEndpoint;
    int requestInterval;
};

// struct to store logging info from config.json
struct logsInfo {
    std::string level;
    bool file;
    bool console;
}; 

// struct symbolInfo to store required data of symbols
struct symbolInfo{
    std::string symbol; 
    std::string quoteAsset; 
    std::string status; 
    std::string tickSize; 
    std::string stepSize;
};

// class stores symbol info for each endpoint in seperate maps
class exchangeInfo{
private:
    std::unordered_map<std::string, symbolInfo> _spotSymbols;
    std::unordered_map<std::string, symbolInfo> _usdSymbols;
    std::unordered_map<std::string, symbolInfo> _coinSymbols;

public:
    // Getter for spotSymbols
    const symbolInfo getSpotSymbol(const std::string&) const;

    // Setter for spotSymbols
    void setSpotSymbol(const std::string&, const symbolInfo&);

    // Getter for usdSymbols
    const symbolInfo getUsdSymbol(const std::string&) const;

    // Setter for usdSymbols
    void setUsdSymbol(const std::string&, const symbolInfo&);

    // Getter for coinSymbols
    const symbolInfo getCoinSymbol(const std::string&) const;

    // Setter for coinSymbols
    void setCoinSymbol(const  std::string&, const symbolInfo&); 

    // Function to get the size of spotSymbols
    const size_t getSpotSymbolsSize() const;

    // Function to get the size of usdSymbols
    const size_t getUsdSymbolsSize() const;

    // Function to get the size of coinSymbols
    const size_t getCoinSymbolsSize() const;

    // Func to update status of spotSymbols
    void updateSpotStatus(const std::string&, const std::string&);
    
    // Func to update status of usdSymbols
    void updateUsdStatus(const std::string&, const std::string&);

    // Func to update status of coinSymbols
    void updateCoinStatus(const std::string&, const std::string&);

    // Func to delete a spot symbol
    void deleteSpotSymbol(const std::string&);

    // Func to delete a USD futures symbol
    void deleteUsdSymbol(const std::string&);

    // Func to delete a coin futures symbol
    void deleteCoinSymbol(const std::string&);

    // check if spot symbol exists
    bool spotSymbolexists(const std::string&) const;

    // check if usd symbol exists
    bool usdSymbolexists(const std::string&) const;

    // check if coin symbol exists
    bool coinSymbolexists(const std::string&) const;

    // configurations functions
    void readConfig(std::string, urlInfo&, logsInfo&);  // Read config file for url info and logs info
    void setSpdLogs(logsInfo&); // set logging level and file/console enabling
    void fetchData(urlInfo&, boost::asio::io_context&, boost::asio::ssl::context&); // get symbols data from endpoints
    void readQuery();   // read query file continously
    void processQuery(std::string&, std::string&, std::string&, std::string&); // process query
};

#endif // BinanceExchange_H