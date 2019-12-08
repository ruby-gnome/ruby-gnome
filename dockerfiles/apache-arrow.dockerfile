FROM ruby:2.6

RUN \
  apt update && \
  apt install -y \
    bison \
    build-essential \
    clang-7 \
    cmake \
    flex \
    g++ \
    gcc \
    git \
    libboost-filesystem-dev \
    libboost-regex-dev \
    libboost-system-dev \
    libbrotli-dev \
    libc-ares-dev \
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
    libzstd-dev \
    ninja-build \
    python3-pip \
    pkg-config \
    protobuf-compiler \
    protobuf-compiler-grpc \
    rapidjson-dev \
    sudo

RUN \
  pip3 install --upgrade meson

RUN \
  useradd --user-group --create-home ruby-gnome

RUN \
  echo "ruby-gnome ALL=(ALL:ALL) NOPASSWD:ALL" | \
    EDITOR=tee visudo -f /etc/sudoers.d/ruby-gnome

COPY . /home/ruby-gnome/ruby-gnome
RUN chown -R ruby-gnome:ruby-gnome /home/ruby-gnome/ruby-gnome

USER ruby-gnome
WORKDIR /home/ruby-gnome/ruby-gnome

RUN bundle install
RUN \
  (cd glib2 && rake gem && gem install --local pkg/*.gem) && \
  (cd gobject-introspection && rake gem && gem install --local pkg/*.gem)

RUN git clone --depth 1 https://github.com/apache/arrow.git ~/arrow
WORKDIR /home/ruby-gnome/arrow
RUN \
  mkdir -p build/cpp && \
  cmake \
    -G Ninja \
    -DARROW_COMPUTE=ON \
    -DARROW_CSV=ON \
    -DARROW_DATASET=ON \
    -DARROW_FILESYSTEM=ON \
    -DARROW_FLIGHT=ON \
    -DARROW_GANDIVA=ON \
    -DARROW_HDFS=ON \
    -DARROW_JSON=ON \
    -DARROW_ORC=ON \
    -DARROW_PARQUET=ON \
    -DARROW_PLASMA=ON \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_INSTALL_LIBDIR=lib \
    -DCMAKE_INSTALL_PREFIX=/usr \
    -DProtobuf_SOURCE=BUNDLED \
    -S cpp \
    -B build/cpp && \
  ninja -C build/cpp && \
  sudo ninja -C build/cpp install
RUN \
  meson \
    --prefix=/usr \
    --libdir=lib \
    --buildtype=debug \
    build/c_glib \
    c_glib && \
  ninja -C build/c_glib && \
  sudo ninja -C build/c_glib install

CMD (cd build/c_glib && ../../c_glib/test/run-test.sh)
