#include "benchmark/benchmark.h"
#include "BinanceExchange.h"
#include "example/common/root_certificates.hpp"
#include <boost/asio/ssl.hpp>

exchangeInfo binanceExchange;
urlInfo urlConfig;
logsInfo logsConfig;

// base URL and endpoint for testing
std::string baseUrl, endpoint;

void initializeConfig() {
    readConfig("config.json", urlConfig, logsConfig);
    baseUrl = urlConfig.spotExchangeBaseUrl;
    endpoint = urlConfig.spotExchangeEndpoint;
}

// Benchmark for the fetchData function
static void BMFetchData(benchmark::State& state) {
    boost::asio::io_context io;
    boost::asio::ssl::context ctx{ssl::context::tlsv12_client};
    load_root_certificates(ctx);
    ctx.set_verify_mode(ssl::verify_peer);
    for (auto _ : state) {
        fetchData(binanceExchange, baseUrl, endpoint, urlConfig, io, ctx);
        io.run();
    }
}
BENCHMARK(BMFetchData);

// Benchmark for the query function
static void BMQuery(benchmark::State& state) {
    setLogLevelForBM();
    std::string market = "SPOT", symbol = "BTCUSDT", type = "GET", status = "";
    for (auto _ : state) {
        query(binanceExchange, market, symbol, type, status);
    }
}
BENCHMARK(BMQuery);

// Main function to run benchmarks
int main() {
    // Read configuration file
    initializeConfig();
    benchmark::RunSpecifiedBenchmarks();
}