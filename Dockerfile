FROM gcc:latest

WORKDIR /app

RUN apt-get update && apt-get install -y \
    libssl-dev \
    cmake \ 
    build-essential

COPY . /app/


# Change to the build directory and run cmake and make
WORKDIR /app/build
RUN rm -rf *
RUN cmake .. && make

# Define the default command to run your application
CMD ["./main"]
