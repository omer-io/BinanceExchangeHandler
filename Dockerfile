FROM gcc:latest

WORKDIR /app

RUN apt-get update && apt-get install -y \
    libssl-dev \
    cmake \ 
    build-essential

COPY . /app/


# Create build directory and copy config.json and query.json into build
RUN mkdir build
RUN cp config.json query.json build/

# run cmake and make
WORKDIR /app/build
RUN cmake ..
RUN make

# run application
CMD ["./main"]
