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
2. Navigate to the build directory: `cd BinanceExchangeHandler`
3. Create a build directory: `mkdir build`
4. Copy config.json and query.json into build: `cp config.json query.json build/`
5. Navigate to the build directory: `cd build`
6. Run CMake: `cmake ..`
7. To disable tests and benchmarks run CMake with options: `cmake .. -DBUILD_TESTS=OFF -DBUILD_BENCHMARKS=OFF`
8. Build the project: `make`
9. Run main app: `./app/main`
10. Run benchmarks: `./benchmark/benchmarks`
11. Run unit tests: `./unittest/test`


To build and run the project in container, follow these steps:

1. Clone the repository: `git clone https://github.com/omer-io/BinanceExchangeHandler`
2. Navigate to the build directory: `cd BinanceExchangeHandler`
3. Create image: `docker build -t binanceimage .`
4. Run contianer `docker run --rm -it -v.:/app --name binancecontainer binanceimage`
5. Create a build directory: `mkdir build`
6. Copy config.json and query.json into build: `cp config.json query.json build/`
7. Navigate to the build directory: `cd build`
8. Run CMake: `cmake ..`
9. To disable tests and benchmarks run CMake with options: `cmake .. -DBUILD_TESTS=OFF -DBUILD_BENCHMARKS=OFF`
10. Build the project: `make`
11. Run main app: `./app/main`
12. Run benchmarks: `./benchmark/benchmarks`
13. Run unit tests: `./unittest/test`
