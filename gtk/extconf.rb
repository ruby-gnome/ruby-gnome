=begin
extconf.rb for Ruby/GTK extention library
=end

PACKAGE_NAME = "gtk2"
PKG_CONFIG_ID = "gtk+-2.0"

TOPDIR = File.expand_path(File.dirname(__FILE__) + '/..')
MKMF_GNOME2_DIR = TOPDIR + '/glib/src/lib'
SRCDIR = TOPDIR + '/gtk/src'

$LOAD_PATH.unshift MKMF_GNOME2_DIR

require 'mkmf-gnome2'

#
# detect GTK+ configurations
#

PKGConfig.have_package('gthread-2.0')
PKGConfig.have_package(PKG_CONFIG_ID) or exit 1
setup_win32(PACKAGE_NAME)

STDOUT.print("checking for target... ")
STDOUT.flush
target = PKGConfig.variable(PKG_CONFIG_ID, "target")
$defs << "-DRUBY_GTK2_TARGET=\\\"#{target}\\\""
STDOUT.print(target, "\n")

#
# detect location of GDK include files
#
gdkincl = nil
tmpincl = $CFLAGS.gsub(/-D\w+/, '').split(/-I/) + ['/usr/include']
tmpincl.each do |i|
  i.strip!
  
  if FileTest.exist?(i + "/gdk/gdkkeysyms.h")
    gdkincl = i + "/gdk"
    break
  end
end
raise "can't find gdkkeysyms.h" if gdkincl.nil?

have_func('gtk_plug_get_type')
have_func('gtk_socket_get_type')
have_func('pango_render_part_get_type')

if target=="x11"
  have_func("XReadBitmapFileData")
  have_header('X11/Xlib.h')
  have_func("XGetErrorText")
end

PKGConfig.have_package('gtk+-unix-print-2.0')

have_func('gtk_print_unix_dialog_get_type')
have_func('gtk_print_job_get_type')
have_func('gtk_printer_get_type')
have_func('gtk_page_setup_unix_get_type')

PKGConfig.have_package('cairo')
if have_header('rb_cairo.h')
  if /mingw|cygwin|mswin32/ =~ RUBY_PLATFORM
    unless ENV["CAIRO_PATH"]
      puts "Error! Set CAIRO_PATH."
      exit 1
    end
    add_depend_package("cairo", "packages/cairo/ext", ENV["CAIRO_PATH"])
    $defs << "-DRUBY_CAIRO_PLATFORM_WIN32"
  end
end

add_depend_package("glib2", "glib/src", TOPDIR)
add_depend_package("pango", "pango/src", TOPDIR)

#
# create Makefiles
#

add_distcleanfile("rbgdkkeysyms.h")
add_distcleanfile("rbgtkinits.c")

create_makefile_at_srcdir(PACKAGE_NAME, SRCDIR, "-DRUBY_GTK2_COMPILATION") {
  File.delete("rbgtkinits.c") if FileTest.exist?("rbgtkinits.c")
  File.delete("rbgtkinits.c") if FileTest.exist?("rbgtkinits.c")
  system("#{$ruby} #{SRCDIR}/makeinits.rb #{SRCDIR}/*.c > rbgtkinits.c") or raise "failed to make GTK inits"
  system("#{$ruby} #{SRCDIR}/makekeysyms.rb #{gdkincl}/gdkkeysyms.h > rbgdkkeysyms.h") or raise "failed to make GDK Keysyms"
}

$defs.delete("-DRUBY_GTK2_COMPILATION")

add_depend_package("gtk2", "gtk/src", TOPDIR)

create_top_makefile(["src"])
