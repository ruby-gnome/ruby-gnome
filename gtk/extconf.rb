=begin
extconf.rb for Ruby/GTK extention library
=end

PACKAGE_NAME = "gtk2"
PKG_CONFIG_ID = "gtk+-2.0"

TOPDIR = File.expand_path(File.dirname(__FILE__) + '/..')
MKMF_GNOME2_DIR = TOPDIR + '/glib/src/lib'
SRCDIR = TOPDIR + '/gtk/src'
SRCDIR20 = TOPDIR + '/gtk/src20'
SRCDIR22 = TOPDIR + '/gtk/src22'

$LOAD_PATH.unshift MKMF_GNOME2_DIR

require 'mkmf-gnome2'

#
# detect GTK+ configurations
#

PKGConfig.have_package('gthread-2.0')
PKGConfig.have_package(PKG_CONFIG_ID) or exit 1
check_win32

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
if target=="x11"
  have_func("XReadBitmapFileData")
  have_header('X11/Xlib.h')
  have_func("XGetErrorText")
end

add_depend_package("glib2", "glib/src", TOPDIR)
add_depend_package("pango", "pango/src", TOPDIR)

#
# create Makefiles
#

begin

  Dir.mkdir('src') unless File.exist? 'src'
  Dir.chdir "src"

  begin
    File.delete("rbgtkinits.c") if FileTest.exist?("rbgtkinits.c")
    File.delete("rbgtkinits.c") if FileTest.exist?("rbgtkinits.c")
    system("ruby #{SRCDIR}/makeinits.rb #{SRCDIR}/*.c > rbgtkinits.c") or raise "failed to make GTK inits"
    system("ruby #{SRCDIR}/makekeysyms.rb #{gdkincl}/gdkkeysyms.h > rbgdkkeysyms.h") or raise "failed to make GDK Keysyms"

    set_output_lib('libruby-gtk2.a')
    $defs << "-DRUBY_GTK2_COMPILATION"

    if $distcleanfiles
      $distcleanfiles << "rbgdkkeysyms.h"
      $distcleanfiles << "rbgtkinits.c"
    end

    create_makefile("gtk2", SRCDIR)
    $defs.delete("-DRUBY_GTK2_COMPILATION")
  ensure
    Dir.chdir ".."
  end

  add_depend_package("gtk2", "gtk/src", TOPDIR)

  Dir.mkdir('src20') unless File.exist? 'src20'
  Dir.chdir "src20"
  begin
    $objs = ["rbgtk20.o"]
    create_makefile("gtk20", SRCDIR20)
  ensure
    Dir.chdir ".."
  end

  Dir.mkdir('src22') unless File.exist? 'src22'
  Dir.chdir "src22"
  begin
    $objs = ["rbgtk22.o"]
    create_makefile("gtk22", SRCDIR22)
  ensure
    Dir.chdir ".."
  end

  create_top_makefile(["src", "src20", "src22"])

rescue Interrupt
  print "  [error] " + $!.to_s + "\n"
end
