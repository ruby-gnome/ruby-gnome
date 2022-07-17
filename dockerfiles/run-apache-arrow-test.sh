#!/bin/bash
#
# Copyright (C) 2020-2022  Ruby-GNOME Project Team
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

set -eux

export CCACHE_DIR=/ruby-gnome/.ccache
ccache -s

mkdir -p ruby-gnome.build
cd ruby-gnome.build

cp /ruby-gnome/Gemfile ./
bundle install

for package in glib2 gobject-introspection; do
  cp -a /ruby-gnome/${package} ./
  pushd ${package}
  rake gem
  gem install pkg/*.gem
  popd
done

git clone --depth 1 https://github.com/apache/arrow.git
mkdir -p arrow.build
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
  -S arrow/cpp \
  -B arrow.build/cpp
ninja -C arrow.build/cpp
ccache -s
sudo ninja -C arrow.build/cpp install

meson \
  --prefix=/usr \
  --libdir=lib \
  --buildtype=debug \
  arrow.build/c_glib \
  arrow/c_glib
ninja -C arrow.build/c_glib
ccache -s
sudo ninja -C arrow.build/c_glib install
ninja -C arrow.build/c_glib test
