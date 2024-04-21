FROM alpine:latest
LABEL authors="yvonm"

# Update the package repository
RUN apk update && apk upgrade

# Install required packages
RUN apk add --no-cache \
    gcc \
    g++ \
    gdb \
    clang \
    make \
    ninja-build \
    cmake \
    autoconf \
    automake \
    libtool \
    valgrind \
    dos2unix \
    rsync \
    tar \
    strace \
    ltrace

# Install custom packages
RUN apk add --no-cache \
    sdl2-dev

# Set the working directory \
WORKDIR /usr/src/app
