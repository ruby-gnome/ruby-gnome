=begin
extconf.rb for Ruby/GLib extention library
=end

TOPDIR = File.expand_path(File.dirname(__FILE__) + '/..')
MKMF_GNOME2_DIR = TOPDIR + '/glib/src/lib'
SRCDIR = TOPDIR + '/glib/src'

$LOAD_PATH.unshift MKMF_GNOME2_DIR

PACKAGE_NAME = "glib2"

require 'mkmf-gnome2'

PKGConfig.have_package('gobject-2.0') or exit 1
PKGConfig.have_package('gthread-2.0')

setup_win32(PACKAGE_NAME)

have_header("unistd.h")
have_header("io.h")

have_func("g_spawn_close_id")
have_func("g_thread_init")
have_func("g_main_depth")
have_func("g_listenv")

have_func("rb_check_array_type")
have_func("rb_exec_recursive")
have_func("rb_errinfo")
have_func("rb_sourcefile")
have_func("rb_sourceline")
have_func("ruby_set_current_source")
have_func("rb_thread_blocking_region")
have_func("ruby_native_thread_p")

have_var("curr_thread", ["ruby.h", "node.h"])
have_var("rb_curr_thread", ["ruby.h", "node.h"])

create_makefile_at_srcdir(PACKAGE_NAME, SRCDIR, "-DRUBY_GLIB2_COMPILATION") do
  enum_type_prefix = "glib-enum-types"
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
  glib_mkenums(enum_type_prefix, headers, "G_TYPE_", ["glib-object.h"])
end

create_top_makefile
