=begin
extconf.rb for Ruby/Pango extention library
=end

PACKAGE_NAME = "pango"

TOPDIR = File.expand_path(File.dirname(__FILE__) + '/..')
MKMF_GNOME2_DIR = TOPDIR + '/glib/src/lib'
SRCDIR = TOPDIR + '/pango/src'

$LOAD_PATH.unshift MKMF_GNOME2_DIR

require 'mkmf-gnome2'

PKGConfig.have_package('pango') or exit 1
setup_win32(PACKAGE_NAME)

have_func("pango_layout_iter_get_type")
have_func("pango_layout_set_ellipsize")
have_func("pango_layout_get_font_description")
have_func("pango_render_part_get_type")

add_depend_package("glib2", "glib/src", TOPDIR)

if $distcleanfiles
  $distcleanfiles << "rbpangoinits.c"
  $distcleanfiles << "rbpangoversion.h"
end

begin
  Dir.mkdir('src') unless File.exist? 'src'
  Dir.chdir "src"

  pango_version = PKGConfig.modversion("pango")

  File.delete("rbpangoinits.c") if FileTest.exist?("rbpangoinits.c")
  File.delete("rbpangoversion.h") if FileTest.exist?("rbpangoversion.h")
  system("ruby #{SRCDIR}/makeinits.rb #{SRCDIR}/*.c > rbpangoinits.c")
  system("ruby #{SRCDIR}/makeversion.rb #{pango_version} > rbpangoversion.h")

  $defs << "-DRUBY_PANGO_COMPILATION"
  create_makefile(PACKAGE_NAME, SRCDIR)
ensure
  Dir.chdir('..')
end

create_top_makefile

