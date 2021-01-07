# Copyright (C) 2002-2021  Ruby-GNOME Project Team
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

require 'pathname'

base_dir = Pathname(__FILE__).dirname.parent.parent.expand_path
mkmf_gnome2_dir = base_dir + 'lib'

$LOAD_PATH.unshift(mkmf_gnome2_dir.to_s)

module_name = "glib2"
package_id = "gobject-2.0"

require "mkmf-gnome"

unless required_pkg_config_package([package_id, 2, 12, 0],
                                   :alt_linux => "glib2-devel",
                                   :debian => "libglib2.0-dev",
                                   :redhat => "pkgconfig(#{package_id})",
                                   :arch_linux => "glib2",
                                   :homebrew => "glib",
                                   :macports => "glib2",
                                   :msys2 => "glib2")
  exit(false)
end
PKGConfig.have_package('gthread-2.0')

have_header("unistd.h")
have_header("io.h")

glib_header = "glib.h"

ruby_header = "ruby.h"
have_func("rb_check_array_type", ruby_header)
have_func("rb_check_hash_type", ruby_header)
have_func("rb_exec_recursive", ruby_header)
have_func("rb_errinfo", ruby_header)
have_func("rb_thread_call_without_gvl", ruby_header)
have_func("ruby_native_thread_p", ruby_header)
have_func("rb_thread_call_with_gvl", ruby_header)
have_func("rb_gc_register_mark_object", ruby_header)
have_func("rb_exc_new_str", ruby_header)
# For Ruby 2.1. TODO: Remove this when we drop Ruby 2.1 support.
have_func("rb_enc_str_new_static", ruby_header)

have_var("curr_thread", [ruby_header, "node.h"])
have_var("rb_curr_thread", [ruby_header, "node.h"])

create_pkg_config_file("Ruby/GLib2", package_id)

enum_types_prefix = "glib-enum-types"
include_paths = PKGConfig.cflags_only_I("glib-2.0")
ignore_headers = [
  "giochannel.h",
  "gmain.h",
  "gscanner.h",
]
unless windows_platform?
  ignore_headers << "gwin32.h"
end
if PKGConfig.check_version?(package_id, 2, 60, 0)
  ignore_headers << "gunicode.h"
end
headers = include_paths.split.inject([]) do |result, path|
  result + Dir.glob(File.join(path.sub(/^-I/, ""), "glib", "*.h"))
end.reject do |file|
  ignore_headers.include?(File.basename(file))
end
include_paths = PKGConfig.cflags_only_I(package_id)
headers = include_paths.split.inject(headers) do |result, path|
  result + Dir.glob(File.join(path.sub(/^-I/, ""), "gobject", "gsignal.h"))
end
glib_mkenums(enum_types_prefix,
             headers,
             "G_TYPE_",
             [],
             preamble: "#include \"rbgprivate.h\"")

$defs << "-DRUBY_GLIB2_COMPILATION"

create_makefile(module_name)

pkg_config_dir = with_config("pkg-config-dir")
if pkg_config_dir.is_a?(String)
  File.open("Makefile", "ab") do |makefile|
    makefile.puts
    makefile.puts("pkgconfigdir=#{pkg_config_dir}")
  end
end
