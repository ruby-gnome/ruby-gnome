=begin
extconf.rb for gnomecanvas extention library
=end

require "mkmf"

begin
	require 'gtk2'
	require 'libart2'
rescue LoadError
	raise "Install gtk2 and libart2 before gnomecanvas2."
end

unless defined? macro_defined?
  def macro_defined?(macro, src, opt="")
    try_cpp(src + <<EOP, opt)
#ifndef #{macro}
# error
#endif
EOP
  end
end

#
# detect GTK+ configurations
#
if /mswin32/ !~ PLATFORM
  config_cmd = with_config("pkg-config", "pkg-config libgnomecanvas-2.0")
  while /^--/ =~ ARGV[0]
    ARGV.shift
  end
  begin
    version = `#{config_cmd} --version`
    if not version.chomp.empty?
      $libs += ' ' + `#{config_cmd} --libs`.chomp
      $CFLAGS += ' ' + `#{config_cmd} --cflags`.chomp
    end
  rescue
    $LDFLAGS = '-L/usr/X11R6/lib -L/usr/local/lib'
    $CFLAGS = '-I/usr/X11R6/lib -I/usr/local/include'
    $libs = '-lm -lc'
  end
else
  $LDFLAGS = '-L/usr/local/lib'
  $CFLAGS = '-I/usr/local/include/gdk/win32 -I/usr/local/include/glib -I/usr/local/include'
end

$CFLAGS = format('-I%s ', File.expand_path(File.dirname(__FILE__) + '/../glib/src')) + $CFLAGS
$CFLAGS = format('-I%s ', File.expand_path(File.dirname(__FILE__) + '/../gtk/src')) + $CFLAGS
$CFLAGS = format('-I%s ', File.expand_path(File.dirname(__FILE__) + '/../libart/src')) + $CFLAGS

STDOUT.print("checking for G_OS_WIN32... ")
STDOUT.flush
if macro_defined?('G_OS_WIN32', "#include <glibconfig.h>\n")
  STDOUT.print "yes\n"
  $CFLAGS += ' -fnative-struct' if /gcc/ =~ Config::CONFIG['CC']
else
  STDOUT.print "no\n"
end

#
# create Makefiles
#
mdir = $mdir
begin
  $mdir = "gnomecanvas/src"
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

  create_makefile("gnomecanvas2")

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
		@cp lib/gnomecanvas2.rb #{rubylibdir}
		@cd src
		@nmake -nologo install DESTDIR=$(DESTDIR)

site-install:
		@cp lib/gnomecanvas2.rb #{sitelibdir}
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
		@cp lib/gnomecanvas2.rb #{rubylibdir}
		@cd src; make install

site-install:
		@cp lib/gnomecanvas2.rb #{sitelibdir}
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
