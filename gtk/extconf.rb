=begin
extconf.rb for gtk extention library
=end

require "mkmf"

#
# detect GTK+ configurations
#
if /mswin32/ !~ PLATFORM
  config_cmd = ENV.fetch("GTK_CONFIG"){
    "pkg-config gtk+-x11-2.0"
  }
  config_libs, config_cflags = "--libs", "--cflags"
  $LDFLAGS, *libs = `#{config_cmd} #{config_libs}`.chomp.split(/(-l.*)/)
  $libs = libs.join(' ') + ' ' + $libs
  $CFLAGS = `#{config_cmd} #{config_cflags}`.chomp
else
  $LDFLAGS = '-L/usr/local/lib'
  $CFLAGS = '-I/usr/local/include/gdk/win32 -I/usr/local/include/glib -I/usr/local/include'
end

#
# detect location of GDK include files
#
gdkincl = nil
tmpincl = $CFLAGS.gsub(/-D\w+/, '').split('-I') + ['/usr/include']
tmpincl.each do |i|
  i.strip!
  
  if FileTest.exist?(i + "/gdk/gdkcursors.h") and
      FileTest.exist?(i + "/gdk/gdkkeysyms.h")
    gdkincl = i + "/gdk"
    break
  end
end
raise "can't found gdkcursors.h or gdkkeysyms.h" if gdkincl.nil?

#
# create Makefiles
#
mdir = $mdir
begin
  $mdir = "gtk/src"
  Dir.chdir "src"

  lib_ary = []
  if /cygwin|mingw/ =~ PLATFORM
    $CFLAGS += " -fnative-struct -DNATIVE_WIN32"
  elsif /mswin32/ !~ PLATFORM
    lib_ary = [ ["X11", "XOpenDisplay"],
                ["Xext", "XShmQueryVersion"],
                ["Xi", "XOpenDevice"],
    ]
  else
    lib_ary = [ ["glib2", "g_print"],
                ["gdk2", "gdk_init"],
                ["gtk2", "gtk_init"] ]
  end

  lib_ary.each do |ary|

    if not have_library(ary[0], ary[1])
      msg = format("cannot found %s in %s.", ary[1], ary[0])
      if ary[0] == "X11"
	msg += " (or maybe `gtk-config --lib' is incorrect...)"
      end
      raise Interrupt, msg
    end
  end
  have_func("XReadBitmapFileData")
  if have_func("gdk_ic_attr_new")
       $CFLAGS = $CFLAGS + " -DUSE_XIM"
  end

  obj_ext = ".#{$OBJEXT}"

  $libs = $libs.split(/\s/).uniq.join(' ')
  $source_files = Dir.glob('*.c')
  $objs = $source_files.collect do |item|
    item.gsub(/\.c$/, obj_ext)
  end

  if /mswin32/ =~ PLATFORM
    $objs << "rbgdkkeysyms.lib"
  else
    $objs << "librbgdkkeysyms.a"
  end

  create_makefile("gtk2")
  raise Interrupt if not FileTest.exist? "Makefile"

  mfile = File.open("Makefile", "a")
  if /mswin32/ =~ PLATFORM
    mfile.puts "	copy /Y  gtk2.lib .."
    mfile.puts
  end
  mfile.print "\n"
  $source_files.each do |e|
    mfile.print "#{e.gsub(/\.c$/, obj_ext)}: #{e} rbgtk.h global.h\n"
  end
  mfile.print "rbgdkconst#{obj_ext}: rbgdkconst.c rbgdkcursors.h\n"
  mfile.print "rbgdk#{obj_ext}: rbgdk.c global.h\n"

  if /mswin32/ =~ PLATFORM
    mfile.print "\
rbgdkkeysyms.lib: makedefconst.rb rbgdkkeysyms.h
	$(RUBY) makedefconst.rb rbgdkkeysyms.h Init_gtk_gdkkeysyms
	cd rbgdkkeysyms
	nmake ..\\$@
	cd ..
"
  else
    mfile.print "\

librbgdkkeysyms.a: makedefconst.rb rbgdkkeysyms.h
	$(RUBY) makedefconst.rb rbgdkkeysyms.h Init_gtk_gdkkeysyms
	cd rbgdkkeysyms; make ../$@; cd ..
"
  end

  mfile.print "\

rbgdkcursors.h:;	$(RUBY) makecursors.rb #{gdkincl}/gdkcursors.h > $@
rbgdkkeysyms.h:;	$(RUBY) makekeysyms.rb #{gdkincl}/gdkkeysyms.h > $@

allclean: clean
	rm -rf rbgdkkeysyms* *.a rbgdkcursors*
"
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
		@-rm -f gtk.lib *~
"
  else
    mfile.print "\

all:
		@cd src; make all
		@if [ ! -r gtk2.a ]; then ln -sf src/gtk2.a gtk2.a; fi

install:;	@cd src; make install
site-install:;	@cd src; make site-install
clean:
		@cd src; make allclean
		@rm -f core gtk2.a *~
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
