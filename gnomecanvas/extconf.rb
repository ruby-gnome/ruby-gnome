=begin
extconf.rb for Ruby/GnomeCanvas extention library
=end

$LOAD_PATH.unshift File.expand_path(File.dirname(__FILE__) + '/../glib/src/lib')

require 'mkmf-gnome2'

#
# detect GTK+ configurations
#

PKGConfig.have_package('libgnomecanvas-2.0') or exit
check_win32

have_func('gnome_canvas_set_center_scroll_region')

top = File.expand_path(File.dirname(__FILE__) + '/..') # XXX
$CFLAGS += " " + ['glib/src', 'gtk/src', 'libart/src'].map{|d|
  "-I" + File.join(top, d)
}.join(" ")

if /cygwin|mingw/ =~ RUBY_PLATFORM
  top = "../.."
  [
    ["glib/src", "ruby-glib2"],
    ["gtk/src", "ruby-gtk2"],
    ["libart/src", "ruby-libart2"],
  ].each{|d,l|
    $libs << " -l#{l}"
    $LDFLAGS << " -L#{top}/#{d}"
  }
end

#
# create Makefiles
#
begin
  srcdir = File.dirname($0) == "." ? "." :
    File.expand_path(File.dirname($0) + "/src")

  Dir.mkdir "src" unless File.exist? "src"
  Dir.chdir "src"

  begin
    obj_ext = ".#{$OBJEXT}"

    $libs = $libs.split(/\s/).uniq.join(' ')
    $source_files = Dir.glob("#{srcdir}/*.c").map{|fname|
      fname[0, srcdir.length+1] = ''
      fname
    }
    $objs = $source_files.collect do |item|
      item.gsub(/\.c$/, obj_ext)
    end

    create_makefile("gnomecanvas2", srcdir)
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
  ensure
    Dir.chdir ".."
  end

  create_top_makefile()

rescue Interrupt
  print "  [error] " + $!.to_s + "\n"
end
