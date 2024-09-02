#include "gtest/gtest.h"
#include "BinanceExchange.h"
#include "example/common/root_certificates.hpp"
#include <boost/asio/ssl.hpp>


// Test fetchData function
TEST(fetchDataFunctionTest, validResponse) {
    exchangeInfo binanceExchange;

    // base URL and endpoint for testing
    std::string baseUrl = "api.binance.com";
    std::string endpoint = "/api/v3/exchangeInfo";

    boost::asio::io_context io;
    boost::asio::ssl::context ctx{ssl::context::tlsv12_client};
    load_root_certificates(ctx);
    ctx.set_verify_mode(ssl::verify_peer);

    // Call fetchData 
    fetchData(binanceExchange, baseUrl, endpoint, io, ctx);
    io.run();

    // check if symbols > 0
    EXPECT_GT(binanceExchange.getSpotSymbolsSize(), 0);

    // check if symbol BTCUSDT has quote asset USDT
    std::string symbolName = "BTCUSDT";
    EXPECT_EQ(binanceExchange.getSpotSymbol(symbolName).quoteAsset, "USDT");
}

// Test for update operation in query function
TEST(queryFunctionTest, updateRequest) {
    exchangeInfo binanceExchange;

    // Add a symbol
    symbolInfo testSymbol;
    testSymbol.symbol = "BTCUSDT";
    testSymbol.quoteAsset = "USDT";
    testSymbol.status = "TRADING";
    testSymbol.tickSize = "0.01";
    testSymbol.stepSize = "0.001";

    binanceExchange.setSpotSymbol(testSymbol.symbol, testSymbol);

    // Update status
    std::string market = "SPOT";
    std::string symbol = "BTCUSDT";
    std::string queryType = "UPDATE";
    std::string queryStatus = "PENDING";

    query(binanceExchange, market, symbol, queryType, queryStatus);

    EXPECT_EQ(binanceExchange.getSpotSymbol(symbol).status, "PENDING");
}

TEST(queryFunctionTest, deleteRequest) {
    exchangeInfo binanceExchange;

    // base URL and endpoint for testing
    std::string baseUrl = "api.binance.com";
    std::string endpoint = "/api/v3/exchangeInfo";

    boost::asio::io_context io;
    boost::asio::ssl::context ctx{ssl::context::tlsv12_client};
    load_root_certificates(ctx);
    ctx.set_verify_mode(ssl::verify_peer);

    // Call fetchData 
    fetchData(binanceExchange, baseUrl, endpoint, io, ctx);
    io.run();

    // Update status
    std::string market = "SPOT";
    std::string symbol = "BTCUSDT";
    std::string queryType = "DELETE";
    std::string queryStatus = "";

    query(binanceExchange, market, symbol, queryType, queryStatus);
    EXPECT_EQ(binanceExchange.spotSymbolexists(symbol), false);
}

int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}