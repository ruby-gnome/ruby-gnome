=begin
extconf.rb for Ruby/GnomePrint extention library
=end
PACKAGE_NAME = "gnomeprint2"

TOPDIR = File.expand_path(File.dirname(__FILE__) + '/..')
MKMF_GNOME2_DIR = TOPDIR + '/glib/src/lib'
SRCDIR = TOPDIR + '/gnomeprint/src'

$LOAD_PATH.unshift MKMF_GNOME2_DIR

require 'mkmf-gnome2'
modname = "libgnomeprint-2.2"

PKGConfig.have_package(modname, 2, 8) or exit 1
setup_win32(PACKAGE_NAME)

add_depend_package("glib2", "glib/src", TOPDIR)
add_depend_package("libart2", "libart/src", TOPDIR)

add_distcleanfile("rbgp-version.h")
version = PKGConfig.modversion(modname).split(/\./)
File.open(File.join("src", "rbgp-version.h"), "w") do |f|
  f.print <<-EOH
#ifndef RBGP_VERSION_H
#define RBGP_VERSION_H

#define LIBGNOMEPRINT_MAJOR_VERSION (#{version[0]})
#define LIBGNOMEPRINT_MINOR_VERSION (#{version[1]})
#define LIBGNOMEPRINT_MICRO_VERSION (#{version[2]})

#define LIBGNOMEPRINT_CHECK_VERSION(major, minor, micro) \\
  (LIBGNOMEPRINT_MAJOR_VERSION > (major) ||              \\
    (LIBGNOMEPRINT_MAJOR_VERSION == (major) &&           \\
      LIBGNOMEPRINT_MINOR_VERSION > (minor)) ||          \\
    (LIBGNOMEPRINT_MAJOR_VERSION == (major) &&           \\
      LIBGNOMEPRINT_MINOR_VERSION == (minor) &&          \\
      LIBGNOMEPRINT_MICRO_VERSION >= (micro)))
#endif
EOH
end

create_makefile_at_srcdir(PACKAGE_NAME, SRCDIR, "-DRUBY_GNOMEPRINT2_COMPILATION")
create_top_makefile
