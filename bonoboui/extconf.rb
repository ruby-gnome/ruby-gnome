=begin
extconf.rb for bonoboui extention library
=end

require "mkmf"

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
# detect BonoboUI configurations
#

begin
  config_cmd = with_config("pkg-config", "pkg-config libgnomeui-2.0 libbonoboui-2.0")
  config_libs = "--libs"
  config_cflags = "--cflags"
  config_library = ""
  version = `#{config_cmd} --version`
  if not version.chomp.empty? then
    $libs += ' ' + `#{config_cmd} #{config_libs} #{config_library}`.chomp
    $CFLAGS += " -I../../glib/src -I../../gtk/src  -I../../gnome/src " +
      `#{config_cmd} #{config_cflags} #{config_library}`.chomp
  else
    raise "Can't find a config command"
  end
rescue
  prefix = '/usr/local'
  $LDFLAGS = '-L#{prefix}/X11R6/lib -L#{prefix}/lib'
  $CFLAGS = '-DORBIT2=1 -D_REENTRANT  -I#{prefix}/include/libgnomeui-2.0 -I#{prefix}/include/libgnome-2.0 -I#{prefix}/include/libgnomecanvas-2.0 -I#{prefix}/include/gtk-2.0 -I#{prefix}/include/libart-2.0 -I#{prefix}/include/gconf/2 -I#{prefix}/include/libbonoboui-2.0 -I#{prefix}/include/glib-2.0 -I#{prefix}/lib/glib-2.0/include -I#{prefix}/include/orbit-2.0 -I#{prefix}/include/libbonobo-2.0 -I#{prefix}/include/gnome-vfs-2.0 -I#{prefix}/lib/gnome-vfs-2.0/include -I#{prefix}/include/linc-1.0 -I#{prefix}/include/bonobo-activation-2.0 -I#{prefix}/include/libxml2 -I#{prefix}/include/pango-1.0 -I#{prefix}/include/freetype2 -I#{prefix}/lib/gtk-2.0/include -I#{prefix}/include/atk-1.0 -I#{prefix}/include/Xft2 -I#{prefix}/X11R6/include '

  $libs = ' -L/usr/X11R6/lib -lgnomeui-2 -lSM -lICE -lbonoboui-2 -lgnomecanvas-2 -lgnome-2 -lart_lgpl_2 -lpangoft2-1.0 -lgtk-x11-2.0 -lgdk-x11-2.0 -latk-1.0 -lgdk_pixbuf-2.0 -lpangoxft-1.0 -lpangox-1.0 -lpango-1.0 -lbonobo-2 -lgconf-2 -lgnomevfs-2 -lbonobo-activation -lORBit-2 -lxml2 -lz -lm -llinc -lgmodule-2.0 -ldl -lgobject-2.0 -lgthread-2.0 -lpthread -lglib-2.0 '
end

STDOUT.print("checking for GCC... ")
STDOUT.flush
if macro_defined?("__GNUC__", "")
  STDOUT.print "yes\n"
  $CFLAGS += ' -Wall' 
  is_gcc = true
else
  STDOUT.print "no\n"
  is_gcc = false
end 

mdir = $mdir
begin
  $mdir = "bonoboui/src"
  Dir.chdir "src"

  File.delete("rbbonobouiinits.c") if FileTest.exist?("rbbonobouiinits.c")

  obj_ext = ".#{$OBJEXT}"

  $libs = $libs.split(/\s/).uniq.join(' ')
  $source_files = Dir.glob('*.c')
  $objs = $source_files.collect do |item|
	item.gsub(/\.c$/, obj_ext)
  end
  $objs << "rbbonobouiinits.o"

  #
  # create Makefiles
  #
  srcdir = File.dirname($0) == "." ? "." : "../src"
  create_makefile("bonoboui2", srcdir)
  raise Interrupt if not FileTest.exist? "Makefile"

  mfile = File.open("Makefile", "a")
  $source_files.each do |e|
	mfile.print "#{e.gsub(/\.c$/, obj_ext)}: #{e} rbbonoboui.h\n"
  end
  mfile.print "\
rbbonobouiinits.c:;    $(RUBY) $(srcdir)/makeinits.rb $(srcdir)/*.c > $@
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
