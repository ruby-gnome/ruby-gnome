#!/bin/bash -l
#
# Copyright (C) 2020-2023  Ruby-GNOME Project Team
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

echo "::group::Prepare build directory"
rm -rf ruby-gnome.build
mkdir -p ruby-gnome.build
cd ruby-gnome.build
echo "::endgroup::"

echo "::group::Install dependencies"
gem install --user-install \
    erb \
    rubygems-requirements-system \
    test-unit \
    webrick
echo "::endgroup::"

MAKEFLAGS="-j$(nproc)" rake -f /ruby-gnome/Rakefile gem:install

if [ "${RUBY_GNOME_TEST_ENABLE:-yes}" != "no" ]; then
  export RUBY_GNOME_BUILD_DIR="${PWD}"
  if type dbus-run-session > /dev/null 2>&1 &&
      type xvfb-run > /dev/null 2>&1; then
    if type gtk4-broadwayd > /dev/null 2>&1; then
      export RUBY_GNOME_GTK4_USE_BROADWAY=yes
    fi
    dbus-run-session \
      xvfb-run --server-args "-screen 0 640x480x24" \
      /ruby-gnome/run-test.rb "$@"
  else
    /ruby-gnome/run-test.rb "$@"
  fi
fi
