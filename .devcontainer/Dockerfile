FROM ubuntu:20.04

ENV TZ=Europe/Paris
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y \
    build-essential \
    cmake \
    valgrind \
    python3 \
    python3-pip \
    graphviz \
    libssl-dev \
    openssl \
    git \
    pkg-config \
    && pip install gprof2dot

WORKDIR /app