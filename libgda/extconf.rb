=begin
extconf.rb for Ruby/Libgda extention library
=end

PACKAGE_NAME = "libgda"

TOPDIR = File.expand_path(File.dirname(__FILE__) + '/..')
MKMF_GNOME2_DIR = TOPDIR + '/glib/src/lib'
SRCDIR = TOPDIR + '/libgda/src'

$LOAD_PATH.unshift MKMF_GNOME2_DIR

require 'mkmf-gnome2'


PKGConfig.have_package("libgda") or exit 1
["1.1", "1.2", "1.3"].each do |version|
    if system("pkg-config libgda --atleast-version=#{version}")
        $CFLAGS << " -DGDA_AT_LEAST_#{version.sub(/\./, "_")} "
    end
end

setup_win32(PACKAGE_NAME)

add_depend_package("glib2", "glib/src", TOPDIR)

create_makefile_at_srcdir(PACKAGE_NAME, SRCDIR, "-DRUBY_LIBGDA_COMPILATION")

create_top_makefile
