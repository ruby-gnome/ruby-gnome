=begin
extconf.rb for gnome extention library
=end

$LOAD_PATH.unshift File.expand_path(File.dirname(__FILE__) + '/..')
require 'mkmf'
require 'mkmf-gnome2'

#
# detect Gnome configurations
#

PKGConfig.have_package('libgnomeui-2.0') or exit
check_win32

$CFLAGS += " -DHAVE_GDKIMLIB -I../../glib/src -I../../gtk/src  -I../../gdkimlib "

mdir = $mdir
begin
  $mdir = "gnome/src"
  Dir.chdir "src"

  lib_ary = [
    ["X11", "XOpenDisplay"],
    ["gnome-2", "gnome_program_init"],
  ]

  lib_ary.each do |ary|

    if not have_library(ary[0], ary[1])
      msg = format("cannot found %s in %s.", ary[1], ary[0])
      if ary[0] == "X11"
        msg += " (or maybe `pkg-config libgnomeui-2.0 --libs' is incorrect...)"
      end
      raise Interrupt, msg
    end
  end

  obj_ext = ".#{$OBJEXT}"

  $libs = $libs.split(/\s/).uniq.join(' ')
  $source_files = Dir.glob('*.c')
  $objs = $source_files.collect do |item|
	item.gsub(/\.c$/, obj_ext)
  end

  #
  # create Makefiles
  #
  srcdir = File.dirname($0) == "." ? "." : "../src"
  create_makefile("gnome2", srcdir)
  raise Interrupt if not FileTest.exist? "Makefile"

  mfile = File.open("Makefile", "a")
  $source_files.each do |e|
	mfile.print "#{e.gsub(/\.c$/, obj_ext)}: #{e} rbgnome.h\n"
  end
  mfile.print "\
allclean: clean
	rm -rf *.a
"
  mfile.close
  Dir.chdir ".."

  mfile = File.open("Makefile", "w")
  mfile.print "\
all:
		@cd src; make all

install:;	@cd src; make install
site-install:;	@cd src; make site-install
clean:
		@cd src; make allclean
distclean:	clean
		@cd src; make distclean
		@rm -f Makefile extconf.h conftest.*
		@rm -f core *~ mkmf.log
"
  mfile.close

rescue Interrupt
#  if $DEBUG
    print "  [error] " + $!.to_s + "\n"
#  end
  Dir.chdir ".."
ensure
  $mdir = mdir
end
