FROM ubuntu:24.04

RUN \
  echo "debconf debconf/frontend select Noninteractive" | \
    debconf-set-selections

RUN \
  apt update && \
  apt install -y -V \
    build-essential \
    bundler \
    ccache \
    clang \
    cmake \
    g++ \
    gcc \
    git \
    libboost-filesystem-dev \
    libboost-regex-dev \
    libboost-system-dev \
    libbrotli-dev \
    libbz2-dev \
    libc-ares-dev \
    libcurl4-openssl-dev \
    libdouble-conversion-dev \
    libgoogle-glog-dev \
    libgrpc++-dev \
    liblz4-dev \
    libprotobuf-dev \
    libprotoc-dev \
    libre2-dev \
    libsnappy-dev \
    libssl-dev \
    libthrift-dev \
    libutf8proc-dev \
    libzstd-dev \
    llvm-dev \
    meson \
    ninja-build \
    python3-pip \
    pkg-config \
    protobuf-compiler-grpc \
    rapidjson-dev \
    ruby-dev \
    sudo \
    tzdata \
    zlib1g-dev

RUN \
  useradd --user-group --create-home ruby-gnome

RUN \
  echo "ruby-gnome ALL=(ALL:ALL) NOPASSWD:ALL" | \
    EDITOR=tee visudo -f /etc/sudoers.d/ruby-gnome

USER ruby-gnome
WORKDIR /home/ruby-gnome
