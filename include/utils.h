#ifndef utils_H
#define utils_H

#include <string>

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

#endif // utils_H