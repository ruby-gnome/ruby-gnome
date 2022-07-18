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

echo "::group::Prepare build directory"
mkdir -p ruby-gnome.build
cd ruby-gnome.build
echo "::endgroup::"

echo "::group::Install dependencies"
cp /ruby-gnome/Gemfile ./
if [[ -n ${SCL:-} ]]; then
  gem install cairo
fi
bundle install
echo "::endgroup::"

ruby /ruby-gnome/extconf.rb --enable-debug-build "$@"
for makefile in */Makefile; do
  target=$(dirname ${makefile})
  echo "::group::Build ${target}"
  make -C ${target} -j$(nproc)
  echo "::endgroup::"
done

export RUBY_GNOME_BUILD_DIR="${PWD}"
if type dbus-run-session > /dev/null 2>&1; then
  dbus-run-session \
    xvfb-run --server-args "-screen 0 640x480x24" \
    /ruby-gnome/run-test.rb "$@"
else
  /ruby-gnome/run-test.rb "$@"
fi
