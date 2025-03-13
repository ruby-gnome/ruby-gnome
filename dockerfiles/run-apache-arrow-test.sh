#!/bin/bash
#
# Copyright (C) 2020-2025  Ruby-GNOME Project Team
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

echo "::group::Prepare ccache"
export CCACHE_DIR=/ruby-gnome/.ccache
ccache -sv
echo "::endgroup::"

echo "::group::Prepare build directory"
rm -rf ruby-gnome.build
mkdir -p ruby-gnome.build
cd ruby-gnome.build
echo "::endgroup::"

echo "::group::Install dependencies"
gem install --user-install \
    rubygems-requirements-system
echo "::endgroup::"

for package in glib2 gobject-introspection; do
  echo "::group::Install ${package}"
  cp -a /ruby-gnome/${package} ./
  pushd ${package}
  rm -rf pkg
  rake gem
  sudo env MAKEFLAGS="-j$(nproc)" gem install pkg/*.gem
  popd
  echo "::endgroup::"
done

echo "::group::Build Apache Arrow C++"
git clone --depth 1 https://github.com/apache/arrow.git
cmake \
  -G Ninja \
  -DARROW_ACERO=ON \
  -DARROW_CSV=ON \
  -DARROW_DATASET=ON \
  -DARROW_FILESYSTEM=ON \
  -DARROW_FLIGHT=ON \
  -DARROW_FLIGHT_SQL=ON \
  -DARROW_GANDIVA=ON \
  -DARROW_GCS=ON \
  -DARROW_HDFS=ON \
  -DARROW_JSON=ON \
  -DARROW_ORC=ON \
  -DARROW_PARQUET=ON \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_INSTALL_LIBDIR=lib \
  -DCMAKE_INSTALL_PREFIX=/usr \
  -DProtobuf_SOURCE=BUNDLED \
  -S arrow/cpp \
  -B arrow.build/cpp
ninja -C arrow.build/cpp
echo "::endgroup::"
echo "::group::Show ccache statistics"
ccache -sv
echo "::endgroup::"
echo "::group::Install Apache Arrow C++"
sudo ninja -C arrow.build/cpp install
echo "::endgroup::"

echo "::group::Build Apache Arrow GLib"
meson \
  --prefix=/usr \
  --libdir=lib \
  --buildtype=debug \
  arrow.build/c_glib \
  arrow/c_glib
ninja -C arrow.build/c_glib
echo "::endgroup::"
echo "::group::Show ccache statistics"
ccache -sv
echo "::endgroup::"
echo "::group::Install Apache Arrow GLib"
sudo ninja -C arrow.build/c_glib install
echo "::endgroup::"
echo "::group::Test Apache Arrow GLib"
ninja -C arrow.build/c_glib test
echo "::endgroup::"
