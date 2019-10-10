#!/usr/bin/env ruby
#
# Copyright (C) 2013-2019  Ruby-GNOME Project Team
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

source_dir = Pathname(__FILE__).dirname
base_dir = source_dir.parent.parent.expand_path
top_dir = base_dir.parent.expand_path
top_build_dir = Pathname(".").parent.parent.parent.expand_path

mkmf_gnome2_dir = top_dir + "glib2" + "lib"
version_suffix = ""
unless mkmf_gnome2_dir.exist?
  if /(-\d+\.\d+\.\d+)(?:\.\d+)?\z/ =~ base_dir.basename.to_s
    version_suffix = $1
    mkmf_gnome2_dir = top_dir + "glib2#{version_suffix}" + "lib"
  end
end

$LOAD_PATH.unshift(mkmf_gnome2_dir.to_s)

module_name = "cairo_gobject"
package_id = "cairo-gobject"

require "mkmf-gnome"

["glib2"].each do |package|
  directory = "#{package}#{version_suffix}"
  build_dir = "#{directory}/tmp/#{RUBY_PLATFORM}/#{package}/#{RUBY_VERSION}"
  add_depend_package(package, "#{directory}/ext/#{package}",
                     top_dir.to_s,
                     :top_build_dir => top_build_dir.to_s,
                     :target_build_dir => build_dir)
end

unless required_pkg_config_package(package_id,
                                   :alt_linux => "libcairo-devel",
                                   :debian => "libcairo2-dev",
                                   :redhat => "pkgconfig(#{package_id})",
                                   :arch_linux => "cairo",
                                   :homebrew => "cairo")
  exit(false)
end

unless check_cairo(:top_dir => top_dir)
  exit(false)
end

create_pkg_config_file("Ruby/CairoGObject",
                       package_id, ruby_gnome_version,
                       "ruby-cairo-gobject.pc")

ensure_objs

$defs << "-DRUBY_CAIRO_GOBJECT_COMPILATION"
create_makefile(module_name)

pkg_config_dir = with_config("pkg-config-dir")
if pkg_config_dir.is_a?(String)
  File.open("Makefile", "ab") do |makefile|
    makefile.puts
    makefile.puts("pkgconfigdir=#{pkg_config_dir}")
  end
end
