#!/usr/bin/env ruby
#
# Copyright (C) 2017-2023  Ruby-GNOME Project Team
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

mkmf_gnome2_dir = top_dir + "glib2" + "lib"
$LOAD_PATH.unshift(mkmf_gnome2_dir.to_s) if mkmf_gnome2_dir.exist?


module_name = "pango"
package_id = "pango"

require "mkmf-gnome"

depended_packages = [
  "glib2",
  "gobject-introspection",
]
depended_packages.each do |package|
  add_depend_package(package,
                     "#{package}/ext/#{package}",
                     top_dir.to_s,
                     top_build_dir: top_build_dir.to_s)
end

unless required_pkg_config_package([package_id, 1, 14, 0],
                                   :alt_linux => "libpango-devel",
                                   :conda => "pango",
                                   :debian => "libpango1.0-dev",
                                   :redhat => "pkgconfig(#{package_id})",
                                   :arch_linux => "pango",
                                   :homebrew => "pango",
                                   :macports => "pango",
                                   :msys2 => "pango")
  exit(false)
end

unless check_cairo(:top_dir => top_dir)
  exit(false)
end

PKGConfig.have_package("pangocairo")

if PKGConfig.have_package("pangowin32")
  $defs << " -DHAVE_PANGO_WIN32"
end
if PKGConfig.have_package("pangoft2")
  $defs << " -DHAVE_PANGO_FT2"
end

PKGConfig.have_package("gobject-introspection-1.0")

pango_header = "pango/pango.h"
have_func("pango_attr_strikethrough_color_new", pango_header)
have_func("pango_attr_underline_color_new", pango_header)
have_func("pango_attr_iterator_get_attrs", pango_header)

create_pkg_config_file("Ruby/Pango", package_id)
$defs << " -DRUBY_PANGO_COMPILATION"
case RUBY_PLATFORM
when /darwin/
  symbols_in_external_bundles = [
    "_rb_gi_struct_get_raw",
    "_rbg_define_method",
    "_rbg_define_singleton_method",
    "_rbg_rval2cstr",
    "_rbgobj_add_relative",
    "_rbgobj_boxed_get",
    "_rbgobj_define_class",
    "_rbgobj_get_enum",
    "_rbgobj_initialize_object",
    "_rbgobj_make_boxed",
    "_rbgobj_make_boxed_raw",
    "_rbgobj_make_enum",
  ]
  symbols_in_external_bundles.each do |symbol|
    $DLDFLAGS << " -Wl,-U,#{symbol}"
  end
end
create_makefile(module_name)

pkg_config_dir = with_config("pkg-config-dir")
if pkg_config_dir.is_a?(String)
  File.open("Makefile", "ab") do |makefile|
    makefile.puts
    makefile.puts("pkgconfigdir=#{pkg_config_dir}")
  end
end
