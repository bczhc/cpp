FROM ubuntu

COPY / /cpp/

WORKDIR /cpp

RUN apt update && \
    export DEBIAN_FRONTEND=noninteractive && \
    apt install g++ cmake ninja-build libssl-dev libsqlite3-dev libpcre3-dev -y

RUN mkdir build && cd build && \
    cmake .. -G ninja && \
    ninja
