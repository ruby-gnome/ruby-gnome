=begin
extconf.rb for Ruby/GtkHtml2 extention library
=end

PACKAGE_NAME = "gtkhtml2"
PACKAGE_ID   = "libgtkhtml-2.0"

TOPDIR = File.expand_path(File.dirname(__FILE__) + '/..')
MKMF_GNOME2_DIR = TOPDIR + '/glib/src/lib'
SRCDIR = TOPDIR + '/gtkhtml2/src'

$LOAD_PATH.unshift MKMF_GNOME2_DIR

require 'mkmf-gnome2'

#
# detect GTK+ configurations
#

PKGConfig.have_package('libgtkhtml-2.0') or exit 1
setup_win32(PACKAGE_NAME)

have_func('html_view_scroll_to_node')
have_func('html_box_block_get_containing_width')
have_func('html_box_embedded_new')
have_func('html_box_table_get_top_offset')
have_func('html_box_table_get_bottom_offset')
#have_func('gnome_canvas_set_center_scroll_region')

add_depend_package("glib2", "glib/src", TOPDIR)
add_depend_package("gtk2", "gtk/src", TOPDIR)

make_version_header("GTKHTML", PACKAGE_ID)

create_makefile_at_srcdir(PACKAGE_NAME, SRCDIR, "-DRUBY_GTKHTML2_COMPILATION")
create_top_makefile
