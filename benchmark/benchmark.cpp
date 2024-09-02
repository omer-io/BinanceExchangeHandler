#include "benchmark/benchmark.h"
#include "BinanceExchange.h"
#include "example/common/root_certificates.hpp"
#include <boost/asio/ssl.hpp>


// Dummy data for benchmarking
std::string dummyBaseUrl = "api.binance.com";
std::string dummyEndpoint = "/api/v3/exchangeInfo";
exchangeInfo dummyExchange;



// Benchmark for the fetchData function
static void BMFetchData(benchmark::State& state) {
    boost::asio::io_context io;
    boost::asio::ssl::context ctx{ssl::context::tlsv12_client};
    load_root_certificates(ctx);
    ctx.set_verify_mode(ssl::verify_peer);
    for (auto _ : state) {
        fetchData(dummyExchange, dummyBaseUrl, dummyEndpoint, io, ctx);
        io.run();
    }
}
BENCHMARK(BMFetchData);

// Benchmark for the query function
static void BMQuery(benchmark::State& state) {
    setLogLevelForBM();
    for (auto _ : state) {
        query(dummyExchange, "SPOT", "BTCUSDT", "GET", "");
    }
}
BENCHMARK(BMQuery);

// Main function to run benchmarks
BENCHMARK_MAIN();
