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
6. Build the project: `make`
7. Run main app: `./app/main`
8. Run benchmarks: `./benchmark/bm`
9. Run unit tests: `./unittest/test`


To build and run the project in container, follow these steps:

1. Clone the repository: `git clone https://github.com/omer-io/BinanceExchangeHandler`
2. Create image: `docker build -t binanceimage .`
3. Run contianer `docker run --rm -it -v.:/app --name binancecontainer binanceimage`
4. Navigate to the build directory: `cd build`
5. Run main app: `./app/main`
6. Run benchmarks: `./benchmark/bm`
7. Run unit tests: `./unittest/test`
