#!/usr/bin/env ruby
#
# Copyright (C) 2015-2023  Ruby-GNOME Project Team
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


module_name = "gtk3"
package_id = "gtk+-3.0"

require "mkmf-gnome"

depended_packages = [
  "glib2",
  "gobject-introspection",
  "cairo-gobject",
  "pango",
]
depended_packages.each do |package|
  add_depend_package(package,
                     "#{package}/ext/#{package}",
                     top_dir.to_s,
                     top_build_dir: top_build_dir.to_s)
end

unless check_cairo(:top_dir => top_dir)
  exit(false)
end

unless required_pkg_config_package(package_id,
                                   :alt_linux => "libgtk+3-devel",
                                   :arch_linux => "gtk3",
                                   :conda => [
                                     "gtk3",
                                     "xorg-xineramaproto",
                                   ],
                                   :debian => "libgtk-3-dev",
                                   :redhat => "pkgconfig(#{package_id})",
                                   :homebrew => "gtk+3",
                                   :macports => "gtk3",
                                   :msys2 => "gtk3")
  exit(false)
end

unless PKGConfig.have_package("gobject-introspection-1.0")
  exit(false)
end

create_pkg_config_file("Ruby/GTK3", package_id)

$defs << "-DRUBY_GTK3_COMPILATION"
case RUBY_PLATFORM
when /darwin/
  symbols_in_external_bundles = [
    "_rb_gi_callback_data_get_rb_callback",
    "_rb_gi_callback_register_finder",
    "_rb_gi_struct_new_raw",
    "_rbg_check_array_type",
    "_rbg_cstr2rval",
    "_rbg_define_private_method",
    "_rbg_define_singleton_method",
    "_rbg_rval2cstr",
    "_rbg_rval_inspect",
    "_rbg_strv2rval",
    "_rbgobj_boxed_not_copy_obj",
    "_rbgobj_gc_mark_gvalue",
    "_rbgobj_gc_mark_instance",
    "_rbgobj_gvalue_to_rvalue",
    "_rbgobj_instance_from_ruby_object",
    "_rbgobj_lookup_class",
    "_rbgobj_make_boxed",
    "_rbgobj_make_flags",
    "_rbgobj_register_mark_func",
    "_rbgobj_ruby_object_from_instance",
    "_rbgobj_rvalue_to_gvalue",
    "_rbgobj_set_signal_call_func",
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
