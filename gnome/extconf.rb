=begin
extconf.rb for gnome extention library
=end

require "mkmf"

#
# detect Gnome configurations
#

begin
  config_cmd = with_config("gnome-config", "gnome-config")
  config_libs = "--libs"
  config_cflags = "--cflags"
  config_library = "gnomeui"
  version = `#{config_cmd} --version`
  if not version.chomp.empty? then
    $libs += ' ' + `#{config_cmd} #{config_libs} #{config_library}`.chomp
    $CFLAGS += " -DHAVE_GDKIMLIB -I../../gtk/src  -I../../gdkimlib " +
      `#{config_cmd} #{config_cflags} #{config_library}`.chomp
  else
    raise "Can't find a config command"
  end
rescue
  $LDFLAGS = '-L/usr/X11R6/lib -L/usr/local/lib'
  $CFLAGS = '-I/usr/X11R6/lib -I/usr/local/include'
  $libs = '-lgnomeui -lart_lgpl -lgdk_imlib -lSM -lICE -lgtk -lgdk -lgmodule -lXext -lX11 -lm -lgnome -lgnomesupport -ldb -lglib -ldl'
end

mdir = $mdir
begin
  $mdir = "gnome/src"
  Dir.chdir "src"

  lib_ary = [ ["X11", "XOpenDisplay"],
              ["Xext", "XShmQueryVersion"],
              ["Xi", "XOpenDevice"],
#              ["glib", "g_print"],
#              ["gdk", "gdk_init"],
#              ["gtk", "gtk_init"],
#              ["gnome", "gnome_init"],
  ]

  lib_ary.each do |ary|

    if not have_library(ary[0], ary[1])
      msg = format("cannot found %s in %s.", ary[1], ary[0])
      if ary[0] == "X11"
        msg += " (or maybe `gnome-config --lib' is incorrect...)"
      end
      raise Interrupt, msg
    end
  end
  have_func("XReadBitmapFileData")

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
  create_makefile("gnome", srcdir)
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
		@if [ ! -r gnome.a ]; then ln -sf src/gnome.a gnome.a; fi 
	
install:;	@cd src; make install
site-install:;	@cd src; make site-install
clean:
		@cd src; make allclean
distclean:	clean
		@cd src; make distclean
		@rm -f Makefile extconf.h conftest.*
		@rm -f core *~ mkmf.log gnome.a
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
