=begin
extconf.rb for gnomecanvas extention library
=end

$LOAD_PATH.unshift File.expand_path(File.dirname(__FILE__) + '/..')
require 'mkmf'
require 'mkmf-gnome2'

#
# detect GTK+ configurations
#

PKGConfig.have_package('libgnomecanvas-2.0') or exit
check_win32

$CFLAGS = format('-I%s ', File.expand_path(File.dirname(__FILE__) + '/../glib/src')) + $CFLAGS
$CFLAGS = format('-I%s ', File.expand_path(File.dirname(__FILE__) + '/../gtk/src')) + $CFLAGS
$CFLAGS = format('-I%s ', File.expand_path(File.dirname(__FILE__) + '/../libart/src')) + $CFLAGS

#
# create Makefiles
#
mdir = $mdir
begin
  $mdir = "gnomecanvas/src"
  src_dir = File.expand_path(File.join(File.dirname(__FILE__), 'src'))
  Dir.mkdir "src" unless File.exist? "src"
  Dir.chdir "src"

  have_func('gnome_canvas_set_center_scroll_region')
  obj_ext = ".#{$OBJEXT}"
  rubylibdir = Config::CONFIG["rubylibdir"]
  sitelibdir = Config::CONFIG["sitelibdir"]

  $libs = $libs.split(/\s/).uniq.join(' ')
  $source_files = Dir.glob('*.c')
  $objs = $source_files.collect do |item|
    item.gsub(/\.c$/, obj_ext)
  end

  create_makefile("gnomecanvas2", src_dir)

  raise Interrupt if not FileTest.exist? "Makefile"

  mfile = File.open("Makefile", "a")
  if /mswin32/ =~ PLATFORM
    mfile.puts "	copy /Y  gnomecanvas2.lib .."
    mfile.puts
  end
  mfile.print "\n"
  $source_files.each do |e|
    mfile.print "#{e.gsub(/\.c$/, obj_ext)}: #{e} rbgnomecanvas.h\n"
  end

  mfile.close
  Dir.chdir ".."

  mfile = File.open("Makefile", "w")
  if /mswin32/ =~ PLATFORM
    mfile.print "\

all:
		@cd src
		@nmake -nologo

install:
		@cd src
		@nmake -nologo install DESTDIR=$(DESTDIR)

site-install:
		@cd src
		@nmake -nologo site-install DESTDIR=$(DESTDIR)

clean:
		@cd src
		@nmake -nologo allclean
		@cd ..
		@-rm -f Makefile extconf.h conftest.*
		@-rm -f gnomecanvas2.lib *~
"
  else
    mfile.print "\

all:
		@cd src; make all

install:
		@cd src; make install

site-install:
		@cd src; make site-install

clean:
		@cd src; make clean
		@rm -f core gnomecanvas2.a *~
distclean:	clean
		@cd src; make distclean
		@rm -f Makefile extconf.h conftest.*
		@rm -f core *~ mkmf.log
"
  end
  mfile.close

rescue Interrupt
#  if $DEBUG
    print "  [error] " + $!.to_s + "\n"
#  end
  Dir.chdir ".."
ensure
  $mdir = mdir
end
