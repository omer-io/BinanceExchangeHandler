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
3. update googletest submodule: `git submodule update --init` 
4. Create a build directory: `mkdir build`
5. Copy config.json and query.json into build: `cp config.json query.json build/`
6. Navigate to the build directory: `cd build`
7. Run CMake: `cmake ..`
8. Build the project: `make`
9. Run main app: `./app/main`
10. Run benchmarks: `./benchmark/benchmarks`
11. Run unit tests: `./unittest/test`


To build and run the project in container, follow these steps:

1. Clone the repository: `git clone https://github.com/omer-io/BinanceExchangeHandler`
2. Navigate to the build directory: `cd BinanceExchangeHandler`
3. update googletest submodule: `git submodule update --init` 
4. Create image: `docker build -t binanceimage .`
5. Run contianer `docker run --rm -it -v.:/app --name binancecontainer binanceimage`
6. Navigate to the build directory: `cd build`
7. Run main app: `./app/main`
8. Run benchmarks: `./benchmark/benchmarks`
9. Run unit tests: `./unittest/test`
