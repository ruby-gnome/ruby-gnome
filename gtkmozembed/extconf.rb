=begin
extconf.rb for Ruby/GtkMozEmbed extention library
=end

PACKAGE_NAME = "gtkmozembed"

TOPDIR = File.expand_path(File.dirname(__FILE__) + '/..')
MKMF_GNOME2_DIR = TOPDIR + '/glib/src/lib'
SRCDIR = TOPDIR + '/gtkmozembed/src'

$LOAD_PATH.unshift MKMF_GNOME2_DIR

require 'mkmf-gnome2'

#
# detect GTK+ configurations
#

PKGConfig.have_package('gtk+-2.0')
PKGConfig.have_package('mozilla-gtkmozembed') 
setup_win32(PACKAGE_NAME)

mozpath = PKGConfig.libs_only_L("mozilla-gtkmozembed")

if mozpath 
  mozpath.sub!(/-L/, " -Wl,-rpath ")
  $LDFLAGS << mozpath
else
  $stderr.puts "mozilla-gtkmozembed.pc cannot be found."
  exit 1
end

add_depend_package("glib2", "glib/src", TOPDIR)
add_depend_package("gtk2", "gtk/src", TOPDIR)

have_library("gtkembedmoz") or exit 1

have_func('gtk_moz_embed_new') or exit 1
have_func('gtk_moz_embed_set_profile_path') or exit 1

create_makefile_at_srcdir(PACKAGE_NAME, SRCDIR, 
                          "-DRUBY_GTKMOZEMBED_COMPILATION")
create_top_makefile
