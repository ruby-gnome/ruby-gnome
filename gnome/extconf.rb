=begin
extconf.rb for Ruby/GNOME extention library
=end

$LOAD_PATH.unshift File.expand_path(File.dirname(__FILE__) + '/../glib/src/lib')

require 'mkmf-gnome2'

#
# detect Gnome configurations
#

PKGConfig.have_package('libgnomeui-2.0') or exit 1
check_win32

top = File.expand_path(File.dirname(__FILE__) + '/..') # XXX
$CFLAGS += " " + ['glib/src', 'gtk/src'].map{|d|
  "-I" + File.join(top, d)
}.join(" ")


begin
  srcdir = File.dirname($0) == "." ? "." :
    File.expand_path(File.dirname($0) + "/src")

  Dir.mkdir "src" unless File.exist? "src"
  Dir.chdir "src"

  begin
    lib_ary = [
      ["X11", "XOpenDisplay"],
      ["gnome-2", "gnome_program_init"],
      ["popt", "poptGetArgs"],
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

    if /cygwin|mingw/ =~ RUBY_PLATFORM
      top = "../.."
      [
    	["glib/src", "ruby-glib2"],
    	["gtk/src", "ruby-gtk2"],
      ].each{|d,l|
    	$libs << " -l#{l}"
    	$LDFLAGS << " -L#{top}/#{d}"
      }
    end

    obj_ext = ".#{$OBJEXT}"

    $libs = $libs.split(/\s/).uniq.join(' ')
    $source_files = Dir.entries(srcdir).select{|fname| /\.c$/ =~ fname }
    $objs = $source_files.collect do |item|
      item.gsub(/\.c$/, obj_ext)
    end

    #
    # create Makefiles
    #
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
  ensure
    Dir.chdir ".."
  end

  create_top_makefile

rescue Interrupt
  print "  [error] " + $!.to_s + "\n"
end
