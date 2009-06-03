=begin
extconf.rb for Ruby/GtkSourceView2 extension library
=end

PACKAGE_NAME = "gtksourceview2"
PACKAGE_ID   = "gtksourceview-2.0"

TOPDIR = File.expand_path(File.dirname(__FILE__) + '/..')
MKMF_GNOME2_DIR = TOPDIR + '/glib/src/lib'
SRCDIR = TOPDIR + '/gtksourceview2/src'

$LOAD_PATH.unshift MKMF_GNOME2_DIR
require 'mkmf-gnome2'

PKGConfig.have_package(PACKAGE_ID) or exit 1
setup_win32(PACKAGE_NAME)

# 2.2
have_func('gtk_source_mark_get_type', "gtksourceview/gtksourcemark.h")
have_func('gtk_source_print_compositor_get_type', "gtksourceview/gtksourceprintcompositor.h")
# 2.4
have_func('gtk_source_view_get_mark_category_background', "gtksourceview/gtksourceview.h")
have_func('gtk_source_language_manager_guess_language', "gtksourceview/gtksourcelanguagemanager.h")

add_depend_package("glib2", "glib/src", TOPDIR)
add_depend_package("gtk2", "gtk/src", TOPDIR)

make_version_header("GTKSOURCEVIEW2", PACKAGE_ID)

create_makefile_at_srcdir(PACKAGE_NAME, SRCDIR, "-DRUBY_GTKSOURCEVIEW2_COMPILATION")
create_pkg_config_file('GTKSOURCEVIEW2', 'src/rbgtksourceview2version.h', 'gtksourceview-2.0-ruby.pc')
create_top_makefile

