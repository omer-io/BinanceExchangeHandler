#ifndef BinanceExchange_H
#define BinanceExchange_H

#include <map>
#include <string>

#include "utils.h"
#include "boost/asio/ssl.hpp"

// class stores symbol info for each endpoint in seperate maps
class exchangeInfo{
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
        
    private:
        std::unordered_map<std::string, symbolInfo> _spotSymbols;
        std::unordered_map<std::string, symbolInfo> _usdSymbols;
        std::unordered_map<std::string, symbolInfo> _coinSymbols;
};

#endif // BinanceExchange_H