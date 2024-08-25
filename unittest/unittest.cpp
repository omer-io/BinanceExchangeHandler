#include <gtest/gtest.h>
#include "exchangeInfoClass.h"

// Test fetchData function
TEST(FetchDataTest, ValidResponse) {
    exchangeInfo binanceExchange;

    // base URL and endpoint for testing
    std::string baseUrl = "api.binance.com";
    std::string endpoint = "/api/v3/exchangeInfo";
    
    // Call fetchData and check if it correctly populates the exchangeInfo object
    fetchData(binanceExchange, baseUrl, endpoint);

    // Example check - you may have different symbols based on the real response
    EXPECT_GT(binanceExchange.spotSymbols.size(), 0);
    EXPECT_EQ(binanceExchange.spotSymbols["BTCUSDT"].quoteAsset, "USDT");
}

// Test for update operation in query function
TEST(QueryFunctionTest, UpdateRequest) {
    exchangeInfo binanceExchange;

    // Add a symbol
    symbolInfo testSymbol;
    testSymbol.symbol = "ETHUSDT";
    testSymbol.quoteAsset = "USDT";
    testSymbol.status = "TRADING";
    testSymbol.tickSize = "0.01";
    testSymbol.stepSize = "0.001";

    binanceExchange.spotSymbols[testSymbol.symbol] = testSymbol;

    // Update status
    std::string market = "SPOT";
    std::string symbol = "ETHUSDT";
    std::string queryType = "UPDATE";
    std::string queryStatus = "PENDING";

    query(binanceExchange, market, symbol, queryType, queryStatus);

    EXPECT_EQ(binanceExchange.spotSymbols[symbol].status, "BREAK");
}

int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}