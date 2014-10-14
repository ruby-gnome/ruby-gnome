#!/bin/sh
#
# Copyright (C) 2013-2014  Ruby-GNOME2 Project Team
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

set -e
set -u

if ! apt-cache show gir1.2-gstreamer-1.0 > /dev/null 2>&1; then
    sudo add-apt-repository --yes ppa:gstreamer-developers/ppa
fi
sudo apt-get update -qq
# TODO: we'll use gir1.2-clutter-gst-2.0 if it is supported in Travis CI.
sudo apt-get install -qq -y \
    libgirepository1.0-dev \
    gstreamer1.0-plugins-good \
    gir1.2-clutter-1.0 \
    gir1.2-clutter-gst-1.0 \
    gir1.2-gtkclutter-1.0 \
    gir1.2-webkit-1.0 \
    gir1.2-webkit-3.0 \
    hicolor-icon-theme \
    gnome-icon-theme
