=begin
extconf.rb for gtk extention library
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
# detect GTK+ configurations
#
if /mswin32/ !~ PLATFORM
  config_cmd = with_config("pkg-config", "pkg-config gtk+-2.0")
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

#
# detect location of GDK include files
#
gdkincl = nil
tmpincl = $CFLAGS.gsub(/-D\w+/, '').split('-I') + ['/usr/include']
tmpincl.each do |i|
  i.strip!
  
  if FileTest.exist?(i + "/gdk/gdkcursor.h") and
      FileTest.exist?(i + "/gdk/gdkkeysyms.h")
    gdkincl = i + "/gdk"
    break
  end
end
raise "can't found gdkcursor.h or gdkkeysyms.h" if gdkincl.nil?

$CFLAGS = format('-I%s ', File.expand_path(File.dirname(__FILE__) + '/../glib/src')) + $CFLAGS
$CFLAGS = format('-I%s ', File.expand_path(File.dirname(__FILE__) + '/../pango/src')) + $CFLAGS

gdkx = `#{config_cmd} --variable=target`.chomp == 'x11'

STDOUT.print("checking for G_OS_WIN32... ")
STDOUT.flush
if macro_defined?('G_OS_WIN32', "#include <glibconfig.h>\n")
  STDOUT.print "yes\n"
  $CFLAGS += ' -fnative-struct' if /gcc/ =~ Config::CONFIG['CC']
else
  STDOUT.print "no\n"
end

STDOUT.print("checking for GCC... ")
if /gcc/ =~ Config::CONFIG['CC']
  STDOUT.print "yes\n"
  $CFLAGS += ' -Wall' 
else
  STDOUT.print "no\n"
end

have_func('gtk_plug_get_type')
have_func('gtk_socket_get_type')
have_func('_gtk_accel_group_attach')
have_func("XReadBitmapFileData")

#
# create Makefiles
#

mdir = $mdir
begin
  $mdir = "gtk/src"
  src_dir = File.expand_path(File.join(File.dirname(__FILE__), 'src'))
  src20_dir = File.expand_path(File.join(File.dirname(__FILE__), 'src20'))
  src22_dir = File.expand_path(File.join(File.dirname(__FILE__), 'src22'))

  Dir.mkdir('src') unless File.exist? 'src'
  Dir.chdir "src"

  obj_ext = ".#{$OBJEXT}"
  rubylibdir = Config::CONFIG["rubylibdir"]
  sitelibdir = Config::CONFIG["sitelibdir"]

  File.delete("rbgtkinits.c") if FileTest.exist?("rbgtkinits.c")
  $libs = $libs.split(/\s/).uniq.join(' ')
  $source_files = Dir.glob(src_dir + '/*.c')
  $source_files.each do |item|
    item.gsub!(Regexp.new("\\A" + Regexp.quote(src_dir) + '/'), '')
  end
  $objs = $source_files.collect do |item|
    item.gsub(/\.c$/, obj_ext)
  end
  $objs << "rbgtkinits.o"

  create_makefile("gtk2", src_dir)

  raise Interrupt if not FileTest.exist? "Makefile"

  mfile = File.open("Makefile", "a")
  if /mswin32/ =~ PLATFORM
    mfile.puts "	copy /Y  gtk2.lib .."
    mfile.puts
  end
  mfile.print "\n"
  $source_files.each do |e|
    if e == "rbgdk.c"
      mfile.print "rbgdk#{obj_ext}: rbgdk.c global.h\n"
    elsif e == "rbgdkcursor.c"
      mfile.print "rbgdkcursor#{obj_ext}: rbgdkcursor.c rbgdkcursor.h\n"
    elsif e == "rbgdkkeyval.c"
      mfile.print "rbgdkkeyval#{obj_ext}: rbgdkkeyval.c rbgdkkeysyms.h\n"
    elsif e == "init.c"
      mfile.print "init#{obj_ext}: init.c rbgtk.h global.h rbgtkinits.c\n"
    else
      mfile.print "#{e.gsub(/\.c$/, obj_ext)}: #{e} rbgtk.h global.h\n"
    end
  end
  mfile.print "rbgtkinits#{obj_ext}: rbgtkinits.c\n"

  mfile.print "\

rbgdkcursor.h:;	$(RUBY) $(srcdir)/makecursors.rb #{gdkincl}/gdkcursor.h > $@
rbgtkinits.c:;	   $(RUBY) $(srcdir)/makeinits.rb $(srcdir)/*.c > $@
rbgdkkeysyms.h:;	$(RUBY) $(srcdir)/makekeysyms.rb #{gdkincl}/gdkkeysyms.h > $@

allclean: clean
	rm -rf rbgdkkeysyms* *.a rbgdkcursors* rbgtkinits*
"
  mfile.close
  Dir.chdir ".."

  $CFLAGS = " -I#{src_dir} " + $CFLAGS

  Dir.mkdir('src20') unless File.exist? 'src20'
  Dir.chdir "src20"
  $objs = ["rbgtk20.o"]
  create_makefile("gtk20", src20_dir)
  Dir.chdir ".."

  Dir.mkdir('src22') unless File.exist? 'src22'
  Dir.chdir "src22"
  $objs = ["rbgtk22.o"]
  create_makefile("gtk22", src22_dir)
  Dir.chdir ".."

  mfile = File.open("Makefile", "w")
  if /mswin32/ =~ PLATFORM
    mfile.print "\

all:
		@cd src
		@nmake -nologo
		@cd ../src20
		@nmake -nologo
		@cd ../src22
		@nmake -nologo

install:
		@cd src
		@nmake -nologo install DESTDIR=$(DESTDIR)
		@cd ../src20
		@nmake -nologo install DESTDIR=$(DESTDIR)
		@cd ../src22
		@nmake -nologo install DESTDIR=$(DESTDIR)

site-install:
		@cd src
		@nmake -nologo site-install DESTDIR=$(DESTDIR)
		@cd ../src20
		@nmake -nologo site-install DESTDIR=$(DESTDIR)
		@cd ../src22
		@nmake -nologo site-install DESTDIR=$(DESTDIR)

clean:
		@cd src
		@nmake -nologo allclean
		@cd ../src20
		@nmake -nologo allclean
		@cd ../src22
		@nmake -nologo allclean
		@cd ..
		@-rm -f Makefile extconf.h conftest.*
		@-rm -f gtk2.lib *~
"
  else
    mfile.print "\

all:
		@cd src; make all
		@cd src20; make all
		@cd src22; make all

install:
		@cd src; make install
		@cd src20; make install
		@cd src22; make install

site-install:;	
		@cd src; make site-install
		@cd src20; make site-install
		@cd src22; make site-install

clean:
		@cd src; make allclean
		@cd src20; make allclean
		@cd src22; make allclean
		@rm -f core gtk2.a *~
distclean:	clean
		@cd src; make distclean
		@cd src20; make distclean
		@cd src22; make distclean
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
