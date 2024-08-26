# Binance Exchange Handler 

## Table of Contents
-----------------

* [Introduction](#introduction)
* [Building and Running](#building-and-running)

## Introduction
---------------

Binance Exchange Handler is a C++ Application that interacts with the Binance cryptocurrency exchange API, retrieves different exchanges information, performs data processing tasks, and utilizes various technologies such as Boost, RapidJSON, CMake, and Docker for enhanced functionality, efficient JSON processing, build management, and containerization.

## Building and Running
---------------------

To build and run the project locally, follow these steps:

1. Clone the repository: `git clone https://github.com/omer-io/BinanceExchangeHandler`
2. Create a build directory: `mkdir build`
3. Move config.json and query.json into build: `mv config.json query.json build/`
4. Navigate to the build directory: `cd build`
5. Run CMake: `cmake ..`
6. To disable tests and benchmarks run CMake with options: `cmake .. -DENABLE_UNITTEST=OFF -DENABLE_BENCHMARK=OFF`
7. Build the project: `make`
8.  Run main app: `./app/main`
9. Run benchmarks: `./benchmark/bm`
10. Run unit tests: `./unittest/test`


To build and run the project locally, follow these steps:

1. Clone the repository: `git clone https://github.com/omer-io/BinanceExchangeHandler`
2. Create image: `docker build -t binanceimage .`
3. Run contianer `docker run --rm -it binanceimage`
4.  Run main app: `./app/main`
5. Run benchmarks: `./benchmark/bm`
6. Run unit tests: `./unittest/test`
