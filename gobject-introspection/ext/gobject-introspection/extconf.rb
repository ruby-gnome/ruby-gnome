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

source_dir = Pathname(__FILE__).dirname
base_dir = source_dir.parent.parent.expand_path
top_dir = base_dir.parent.expand_path
top_build_dir = Pathname(".").parent.parent.parent.expand_path

mkmf_gnome2_dir = top_dir + "glib2" + "lib"
$LOAD_PATH.unshift(mkmf_gnome2_dir.to_s) if mkmf_gnome2_dir.exist?


module_name = "gobject_introspection"
package_id = "gobject-introspection-1.0"

require "mkmf-gnome"

["glib2"].each do |package|
  add_depend_package(package,
                     "#{package}/ext/#{package}",
                     top_dir.to_s,
                     top_build_dir: top_build_dir.to_s)
end

unless required_pkg_config_package(package_id,
                                   :alt_linux => "gobject-introspection-devel",
                                   :conda => "gobject-introspection",
                                   :debian => "libgirepository1.0-dev",
                                   :redhat => "pkgconfig(gobject-introspection-1.0)",
                                   :homebrew => "gobject-introspection",
                                   :arch_linux => "gobject-introspection",
                                   :macports => "gobject-introspection",
                                   :msys2 => "gobject-introspection")
  exit(false)
end

# TODO: Remove this when we dropped support for GObject Introspection < 1.60
make_version_header("GI", package_id, ".")

enum_type_prefix = "gobject-introspection-enum-types"
include_paths = PKGConfig.cflags_only_I(package_id)
headers = include_paths.split.inject([]) do |result, path|
  result + Dir.glob(File.join(path.sub(/^-I/, ""), "gi{repository,types}.h"))
end
glib_mkenums(enum_type_prefix, headers, "G_TYPE_", ["girepository.h"])

create_pkg_config_file("Ruby/GObjectIntrospection",
                       package_id, ruby_gnome_version,
                       "ruby-gobject-introspection.pc")

ensure_objs

$defs << "-DRUBY_GOBJECT_INTROSPECTION_COMPILATION"
case RUBY_PLATFORM
when /darwin/
  symbols_in_external_bundles = [
    "_g_rclosure_attach",
    "_g_rclosure_attach_gobject",
    "_g_rclosure_new",
    "_g_signal_flags_get_type",
    "_rbg_cGLibObject",
    "_rbg_cstr2rval",
    "_rbg_cstr2rval_free",
    "_rbg_define_method",
    "_rbg_define_singleton_method",
    "_rbg_filename_from_ruby",
    "_rbg_filename_to_ruby",
    "_rbg_glist2rval",
    "_rbg_glist2rval_with_type",
    "_rbg_gslist2rval",
    "_rbg_gslist2rval_with_type",
    "_rbg_inspect",
    "_rbg_is_bytes",
    "_rbg_is_object",
    "_rbg_is_value",
    "_rbg_mGLib",
    "_rbg_rval2cstr",
    "_rbg_rval2cstr_accept_nil",
    "_rbg_rval2cstr_accept_symbol",
    "_rbg_rval2filenamev",
    "_rbg_rval2glist",
    "_rbg_rval2gslist",
    "_rbg_rval2strv",
    "_rbg_rval2strv_dup",
    "_rbg_scan_options",
    "_rbg_strv2rval",
    "_rbg_to_array",
    "_rbg_variant_from_ruby",
    "_rbg_variant_to_ruby",
    "_rbgerr_define_gerror",
    "_rbgerr_gerror2exception",
    "_rbgerr_ruby_error_quark",
    "_rbgobj_add_relative",
    "_rbgobj_boxed_get",
    "_rbgobj_boxed_unown",
    "_rbgobj_constant_remap",
    "_rbgobj_convert_define",
    "_rbgobj_define_class",
    "_rbgobj_get_enum",
    "_rbgobj_get_flags",
    "_rbgobj_get_ruby_object_from_gobject",
    "_rbgobj_gobject_initialize",
    "_rbgobj_gtype_from_ruby",
    "_rbgobj_gtype_new",
    "_rbgobj_initialize_gvalue",
    "_rbgobj_initialize_object",
    "_rbgobj_instance_from_ruby_object",
    "_rbgobj_instance_unref",
    "_rbgobj_lookup_class",
    "_rbgobj_make_boxed",
    "_rbgobj_make_boxed_default",
    "_rbgobj_make_boxed_raw",
    "_rbgobj_make_enum",
    "_rbgobj_make_flags",
    "_rbgobj_object_add_relative",
    "_rbgobj_object_alloc_func",
    "_rbgobj_object_remove_relative",
    "_rbgobj_remove_relative",
    "_rbgobj_ruby_object_from_instance",
    "_rbgobj_ruby_object_from_instance2",
    "_rbgutil_invoke_callback",
    "_rbgutil_on_callback_error",
    "_rbgutil_start_callback_dispatch_thread",
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
