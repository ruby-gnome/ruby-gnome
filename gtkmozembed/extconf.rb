=begin
extconf.rb for Ruby/GtkMozEmbed extention library
=end

PACKAGE_NAME = "gtkmozembed"
PACKAGE_ID = "xulrunner-gtkmozembed"
#PACKAGE_ID = "firefox-gtkmozembed"
#PACKAGE_ID = "mozilla-gtkmozembed"

TOPDIR = File.expand_path(File.dirname(__FILE__) + '/..')
MKMF_GNOME2_DIR = TOPDIR + '/glib/src/lib'
SRCDIR = TOPDIR + '/gtkmozembed/src'

$LOAD_PATH.unshift MKMF_GNOME2_DIR

require 'mkmf-gnome2'

#
# detect GTK+ configurations
#

PKGConfig.have_package('gtk+-2.0')
PKGConfig.have_package(PACKAGE_ID)
setup_win32(PACKAGE_NAME)

mozpath = PKGConfig.libs_only_L(PACKAGE_ID)
mozpath.sub!(/-L/, "")

if mozpath 
  # please comment the CFLAGS line if you don't want a default comp_path
  $CFLAGS << "'-DDEFAULT_MOZILLA_FIVE_HOME=\"" << mozpath << "\"'"
  $LDFLAGS << " -Wl,-rpath " << mozpath
else
  $stderr.puts "${PACKAGE_ID}.pc cannot be found."
  exit 1
end

add_depend_package("glib2", "glib/src", TOPDIR)
add_depend_package("gtk2", "gtk/src", TOPDIR)

have_library("gtkembedmoz") or exit 1

have_func('gtk_moz_embed_new') or exit 1
have_func('gtk_moz_embed_set_profile_path') or exit 1

make_version_header("GTKMOZEMBED", PACKAGE_ID)

create_makefile_at_srcdir(PACKAGE_NAME, SRCDIR, 
                          "-DRUBY_GTKMOZEMBED_COMPILATION")
create_top_makefile
