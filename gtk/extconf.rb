=begin
extconf.rb for Ruby/GTK extention library
=end

$LOAD_PATH.unshift File.expand_path(File.dirname(__FILE__) + '/../glib/src/lib')

require 'mkmf-gnome2'

#
# detect GTK+ configurations
#

PKGConfig.have_package('gthread-2.0')
pkgname= 'gtk+-2.0'
PKGConfig.have_package(pkgname) or exit
check_win32

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
have_func("XReadBitmapFileData")
have_header('X11/Xlib.h')
have_func("XGetErrorText")

top = File.expand_path(File.dirname(__FILE__) + '/..') # XXX
$CFLAGS += " " + ['glib/src', 'pango/src'].map{|d|
  "-I" + File.join(top, d)
}.join(" ")

if /cygwin|mingw/ =~ RUBY_PLATFORM
  top = "../.."
  [
    ["glib/src", "ruby-glib2"],
    ["pango/src", "ruby-pango"],
  ].each{|d,l|
    $LDFLAGS << " -L#{top}/#{d}"
    $libs << " -l#{l}"
  }
end

#
# create Makefiles
#

begin
  src_dir   = File.expand_path(File.dirname(__FILE__) + '/src')
  src20_dir = File.expand_path(File.dirname(__FILE__) + '/src20')
  src22_dir = File.expand_path(File.dirname(__FILE__) + '/src22')

  Dir.mkdir('src') unless File.exist? 'src'
  Dir.chdir "src"

  begin
    obj_ext = ".#{$OBJEXT}"

    File.delete("rbgtkinits.c") if FileTest.exist?("rbgtkinits.c")
    $libs = $libs.split(/\s/).uniq.join(' ')
    $source_files = Dir.glob("#{src_dir}/*.c").map{|fname|
      fname[0, src_dir.length+1] = ''
      fname
    }
    $objs = $source_files.collect do |item|
      item.gsub(/\.c$/, obj_ext)
    end
    $objs << "rbgtkinits.o"

    
    set_output_lib('libruby-gtk2.a')
    $defs << "-DRUBY_GTK2_COMPILATION"
    create_makefile("gtk2", src_dir)
    $defs.delete("-DRUBY_GTK2_COMPILATION")
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

rbgtkinits.c:;	   $(RUBY) $(srcdir)/makeinits.rb $(srcdir)/*.c > $@
rbgdkkeysyms.h:;	$(RUBY) $(srcdir)/makekeysyms.rb #{gdkincl}/gdkkeysyms.h > $@

allclean: clean
	rm -rf rbgdkkeysyms* *.a rbgtkinits*
"
    mfile.close
  ensure
    Dir.chdir ".."
  end


  $CFLAGS = " -I#{src_dir} " + $CFLAGS
  if /cygwin|mingw/ =~ RUBY_PLATFORM
    $LDFLAGS << " -L../src"
    $libs << " -lruby-gtk2"
  end
  set_output_lib(nil)


  Dir.mkdir('src20') unless File.exist? 'src20'
  Dir.chdir "src20"
  begin
    $objs = ["rbgtk20.o"]
    create_makefile("gtk20", src20_dir)
  ensure
    Dir.chdir ".."
  end

  Dir.mkdir('src22') unless File.exist? 'src22'
  Dir.chdir "src22"
  begin
    $objs = ["rbgtk22.o"]
    create_makefile("gtk22", src22_dir)
  ensure
    Dir.chdir ".."
  end

  create_top_makefile(["src", "src20", "src22"])

rescue Interrupt
  print "  [error] " + $!.to_s + "\n"
end
