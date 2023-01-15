#!/usr/bin/env ruby
#
# Copyright (C) 2012-2023  Ruby-GNOME Project Team
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

require "pathname"

base_dir = Pathname(__FILE__).dirname.parent.parent.expand_path
top_dir = base_dir.parent
top_build_dir = Pathname(".").parent.parent.parent.expand_path

mkmf_gnome2_dir = top_dir + "glib2" + 'lib'
$LOAD_PATH.unshift(mkmf_gnome2_dir.to_s) if mkmf_gnome2_dir.exist?


module_name = "gvlc"
package_id = "libvlc"

require "mkmf-gnome"

["glib2"].each do |package|
  add_depend_package(package,
                     "#{package}/ext/#{package}",
                     top_dir.to_s,
                     top_build_dir: top_build_dir.to_s)
end

unless required_pkg_config_package([package_id, 2, 0, 0],
                                   :alt_linux => "libvlc-devel",
                                   :debian => "libvlc-dev",
                                   :msys2 => "vlc")
  exit(false)
end

unless PKGConfig.have_package("gobject-2.0")
  exit(false)
end

create_pkg_config_file("Ruby/VLC", package_id)

$defs << "-DRUBY_VLC_COMPILATION"
create_makefile(module_name)

pkg_config_dir = with_config("pkg-config-dir")
if pkg_config_dir.is_a?(String)
  File.open("Makefile", "ab") do |makefile|
    makefile.puts
    makefile.puts("pkgconfigdir=#{pkg_config_dir}")
  end
end
