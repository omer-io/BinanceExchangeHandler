FROM ubuntu:latest

RUN apt-get update 
RUN apt-get install libssl-dev -y
RUN apt-get install vim nano -y
RUN apt-get install build-essential -y
RUN apt-get install git-all -y
RUN apt-get install cmake -y

WORKDIR /app


