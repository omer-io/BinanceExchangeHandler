#include <map>
#include <string>

// struct symbolInfo to store required data of symbols
struct symbolInfo{
    std::string symbol, quoteAsset, status, tickSize, stepSize;
};

// class stores symbol info for each endpoint in seperate maps
class exchangeInfo{
    public:
        std::map<std::string, symbolInfo> spotSymbols;
        std::map<std::string, symbolInfo> usdSymbols;
        std::map<std::string, symbolInfo> coinSymbols;
};

// functions declared here are defined in src/getApiData.cpp
void fetchData(exchangeInfo&, std::string, std::string);
void readQuery(exchangeInfo&);
void query(exchangeInfo&, std::string, std::string, std::string, std::string);
void setLogLevelForBM();