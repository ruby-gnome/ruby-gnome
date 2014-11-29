#!/bin/sh

export BUILD_HOST="x86_64-w64-mingw32"
export DIRECTORY_SUFFIX="win64"

export RCAIRO_WINDOWS_64=yes
export RUBY_GNOME2_BUILD_ARCHITECTURE=x64

/vagrant/build-windows.sh
