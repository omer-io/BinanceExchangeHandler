#ifndef BinanceExchange
#define BinanceExchange

#include <map>
#include <string>
#include <boost/asio/ssl.hpp>

// struct to store base url and endpoints info
struct urlInfo{
    std::string spotExchangeBaseUrl, usdFutureExchangeBaseUrl, coinFutureExchangeBaseUrl;
    std::string spotExchangeEndpoint, usdFutureEndpoint, coinFutureEndpoint;
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
    std::string symbol, quoteAsset, status, tickSize, stepSize;
};

// class stores symbol info for each endpoint in seperate maps
class exchangeInfo{
private:
    std::map<std::string, symbolInfo> spotSymbols;
    std::map<std::string, symbolInfo> usdSymbols;
    std::map<std::string, symbolInfo> coinSymbols;

public:
    // Getter for spotSymbols
    symbolInfo getSpotSymbol(std::string&) const;

    // Setter for spotSymbols
    void setSpotSymbol(std::string&, symbolInfo&);

    // Getter for usdSymbols
    symbolInfo getUsdSymbol(std::string&) const;

    // Setter for usdSymbols
    void setUsdSymbol(std::string&, symbolInfo&);

    // Getter for coinSymbols
    symbolInfo getCoinSymbol(std::string&) const;

    // Setter for coinSymbols
    void setCoinSymbol( std::string&, symbolInfo&); 

    // Function to get the size of spotSymbols
    size_t getSpotSymbolsSize() const;

    // Function to get the size of usdSymbols
    size_t getUsdSymbolsSize() const;

    // Function to get the size of coinSymbols
    size_t getCoinSymbolsSize() const;

    // Func to update status of spotSymbols
    void updateSpotStatus(std::string&, std::string&);
    
    // Func to update status of usdSymbols
    void updateUsdStatus(std::string&, std::string&);

    // Func to update status of coinSymbols
    void updateCoinStatus(std::string&, std::string&);

    // Func to delete a spot symbol
    void deleteSpotSymbol(std::string&);

    // Func to delete a USD futures symbol
    void deleteUsdSymbol(std::string&);

    // Func to delete a coin futures symbol
    void deleteCoinSymbol(std::string&);

    // check if spot symbol exists
    bool spotSymbolexists(std::string&) const;

    // check if usd symbol exists
    bool usdSymbolexists(std::string&) const;

    // check if coin symbol exists
    bool coinSymbolexists(std::string&) const;

    // configurations functions
    void readConfig(std::string, urlInfo&, logsInfo&);
    void setSpdLogs(logsInfo&);
    void fetchData(urlInfo&, boost::asio::io_context&, boost::asio::ssl::context&);
    void readQuery();
    void query(std::string&, std::string&, std::string&, std::string&);
};

#endif // BinanceExchange