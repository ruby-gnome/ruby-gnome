=begin
extconf.rb for GtkSourceView extention library
=end

$LOAD_PATH.unshift File.expand_path(File.dirname(__FILE__) + '/..')
require 'mkmf'
require 'mkmf-gnome2'

#
# detect configurations
#

PKGConfig.have_package('gtksourceview-1.0') or exit
check_win32

top = File.expand_path(File.dirname(__FILE__) + '/..')
$CFLAGS += " " + ['glib/src'].map{|d|
  "-I" + File.join(top, d)
}.join(" ")

set_output_lib('libruby-gtksourceview.a')
if /cygwin|mingw/ =~ RUBY_PLATFORM
  top = "../.."
  [
    ["glib/src", "ruby-glib2"],
  ].each{|d,l|
    $LDFLAGS << " -L#{top}/#{d}"
    $libs << " -l#{l}"
  }
end

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

    #
    # create Makefiles
    #
    $defs << "-DRUBY_GTKSOURCEVIEW_COMPILATION"
    create_makefile("gtksourceview", srcdir)
    raise Interrupt if not FileTest.exist? "Makefile"

    mfile = File.open("Makefile", "a")
    $source_files.each do |e|
      mfile.print "#{e.gsub(/\.c$/, obj_ext)}: #{e} rbgtksourcemain.h \n"
    end
    mfile.close
  ensure
    Dir.chdir ".."
  end

  create_top_makefile()
rescue Interrupt
  print "  [error] " + $!.to_s + "\n"
end
