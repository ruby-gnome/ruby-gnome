#!/usr/bin/env ruby
#
# Copyright (C) 2013-2023  Ruby-GNOME Project Team
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
$LOAD_PATH.unshift(mkmf_gnome2_dir.to_s) if mkmf_gnome2_dir.exist?


module_name = "cairo_gobject"
package_id = "cairo-gobject"

require "mkmf-gnome"

["glib2"].each do |package|
  add_depend_package(package,
                     "#{package}/ext/#{package}",
                     top_dir.to_s,
                     top_build_dir: top_build_dir.to_s)
end

unless required_pkg_config_package(package_id,
                                   :alt_linux => "libcairo-devel",
                                   :conda => "cairo",
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
case RUBY_PLATFORM
when /darwin/
  symbols_in_external_bundles = [
    "_rb_cCairo_Context",
    "_rb_cCairo_Device",
    "_rb_cCairo_FontFace",
    "_rb_cCairo_FontOptions",
    "_rb_cCairo_Pattern",
    "_rb_cCairo_Region",
    "_rb_cCairo_ScaledFont",
    "_rb_cCairo_Surface",
    "_rb_cairo_context_from_ruby_object",
    "_rb_cairo_context_to_ruby_object",
    "_rb_cairo_device_from_ruby_object",
    "_rb_cairo_device_to_ruby_object",
    "_rb_cairo_font_face_from_ruby_object",
    "_rb_cairo_font_face_to_ruby_object",
    "_rb_cairo_font_options_from_ruby_object",
    "_rb_cairo_font_options_to_ruby_object",
    "_rb_cairo_pattern_from_ruby_object",
    "_rb_cairo_pattern_to_ruby_object",
    "_rb_cairo_region_from_ruby_object",
    "_rb_cairo_region_to_ruby_object",
    "_rb_cairo_scaled_font_from_ruby_object",
    "_rb_cairo_scaled_font_to_ruby_object",
    "_rb_cairo_surface_from_ruby_object",
    "_rb_cairo_surface_to_ruby_object",
    "_rbgobj_convert_define",
    "_rbgobj_define_class",
    "_rbgobj_gtype_new",
    "_rbgobj_make_boxed",
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
