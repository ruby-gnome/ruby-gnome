#!/bin/bash
#
# Copyright (C) 2020  Ruby-GNOME Project Team
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

mkdir -p ruby-gnome.build
cd ruby-gnome.build

cp /ruby-gnome/Gemfile ./
bundle install

for package in glib2 gobject-introspection; do
  cp -a /ruby-gnome/${package} ./
  pushd ${package}
  rake gem
  gme install pkg/*.gem
done

meson \
  --prefix=/usr \
  --libdir=lib \
  --buildtype=debug \
  arrow-glib \
  /arrow/c_glib
ninja -C arrow-glib
sudo ninja -C arrow-glib install
ninja -C arrow-glib test
