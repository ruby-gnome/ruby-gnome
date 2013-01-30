=begin
extconf.rb for Ruby/GLib extention library
=end

require 'pathname'

base_dir = Pathname(__FILE__).dirname.parent.parent.expand_path
mkmf_gnome2_dir = base_dir + 'lib'

$LOAD_PATH.unshift(mkmf_gnome2_dir.to_s)

module_name = "glib2"
package_id = "gobject-2.0"

require 'mkmf-gnome2'

setup_win32(module_name, base_dir)

unless required_pkg_config_package(package_id,
                                   :debian => "libglib2.0-dev",
                                   :redhat => "glib2-devel",
                                   :homebrew => "glib",
                                   :macports => "glib2")
  exit(false)
end
PKGConfig.have_package('gthread-2.0')

have_header("unistd.h")
have_header("io.h")

glib_header = "glib.h"
have_func("g_spawn_close_pid", glib_header)
have_func("g_thread_init", glib_header)
have_func("g_main_depth", glib_header)
have_func("g_listenv", glib_header)

ruby_header = "ruby.h"
have_func("rb_check_array_type", ruby_header)
have_func("rb_exec_recursive", ruby_header)
have_func("rb_errinfo", ruby_header)
have_func("rb_sourcefile", ruby_header)
have_func("rb_sourceline", ruby_header)
have_func("ruby_set_current_source", ruby_header)
have_func("rb_thread_blocking_region", ruby_header)
have_func("ruby_native_thread_p", ruby_header)
have_func("rb_thread_call_with_gvl", ruby_header)
have_func("rb_str_new_cstr", ruby_header)
have_func("rb_gc_register_mark_object", ruby_header)

have_var("curr_thread", [ruby_header, "node.h"])
have_var("rb_curr_thread", [ruby_header, "node.h"])

create_pkg_config_file("Ruby/GLib2", package_id)

enum_types_prefix = "glib-enum-types"
include_paths = PKGConfig.cflags_only_I("glib-2.0")
headers = include_paths.split.inject([]) do |result, path|
  result + Dir.glob(File.join(path.sub(/^-I/, ""), "glib", "*.h"))
end.reject do |file|
  /g(iochannel|scanner)\.h/ =~ file
end
include_paths = PKGConfig.cflags_only_I("gobject-2.0")
headers = include_paths.split.inject(headers) do |result, path|
  result + Dir.glob(File.join(path.sub(/^-I/, ""), "gobject", "gsignal.h"))
end
glib_mkenums(enum_types_prefix, headers, "G_TYPE_", ["glib-object.h"])

$defs << "-DRUBY_GLIB2_COMPILATION"
create_makefile(module_name)

pkg_config_dir = with_config("pkg-config-dir")
if pkg_config_dir.is_a?(String)
  File.open("Makefile", "ab") do |makefile|
    makefile.puts
    makefile.puts("pkgconfigdir=#{pkg_config_dir}")
  end
end
