#include <benchmark/benchmark.h>
#include "exchangeInfoClass.h"


// Dummy data for benchmarking
std::string dummyBaseUrl = "api.binance.com";
std::string dummyEndpoint = "/api/v3/exchangeInfo";
exchangeInfo dummyExchange;

// Benchmark for the fetchData function
static void BMFetchData(benchmark::State& state) {
    for (auto _ : state) {
        fetchData(dummyExchange, dummyBaseUrl, dummyEndpoint);
    }
}
BENCHMARK(BMFetchData);

// // Benchmark for the query function
// static void BMQuery(benchmark::State& state) {
//     for (auto _ : state) {
//         query(dummyExchange, "SPOT", "BTCUSDT", "GET", "");
//     }
// }
// BENCHMARK(BMQuery);

// Main function to run benchmarks
BENCHMARK_MAIN();
