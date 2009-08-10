=begin
extconf.rb for Ruby/GnomePrintUI extention library
=end

require "fileutils"

PACKAGE_NAME = "gnomeprintui2"
PACKAGE_ID   = "libgnomeprintui-2.2"

TOPDIR = File.expand_path(File.dirname(__FILE__) + '/..')
MKMF_GNOME2_DIR = TOPDIR + '/glib/src/lib'
SRCDIR = TOPDIR + '/gnomeprintui/src'

$LOAD_PATH.unshift MKMF_GNOME2_DIR

require 'mkmf-gnome2'

app_name = "LIBGNOMEPRINTUI"

PKGConfig.have_package(PACKAGE_ID) or exit 1
setup_win32(PACKAGE_NAME)

add_depend_package("glib2", "glib/src", TOPDIR)
add_depend_package("gtk2", "gtk/src", TOPDIR)
add_depend_package("libart2", "libart/src", TOPDIR)
add_depend_package("gnomeprint2", "gnomeprint/src", TOPDIR)

have_header("libgnomeprintui/gnome-print-config-dialog.h")
have_header("libgnomeprintui/gnome-font-dialog.h")

make_version_header(app_name, PACKAGE_ID)

create_makefile_at_srcdir(PACKAGE_NAME, SRCDIR,
                          "-DRUBY_GNOMEPRINTUI2_COMPILATION") do
  enum_type_dir = "libgnomeprintui"
  enum_type_prefix = "libgnomeprintui-enum-types"
  unless have_header("#{enum_type_dir}/#{enum_type_prefix}.h")
    include_paths = `pkg-config libgnomeprintui-2.2 --cflags-only-I`
    include_path = include_paths.split.find do |x|
      /libgnomeprintui/.match(x)
    end.sub(/^-I/, "")
    headers = Dir.glob(File.join(include_path, "libgnomeprintui", "*.h"))

    glib_mkenums(enum_type_prefix, headers, "GNOME_TYPE_PRINT_",
                 ["libgnomeprintui/gnome-print-dialog.h",
                  "libgnomeprintui/gnome-print-paper-selector.h"])
  end
end

create_pkg_config_file(PACKAGE_NAME, PACKAGE_ID, ruby_gnome2_version)
create_top_makefile
