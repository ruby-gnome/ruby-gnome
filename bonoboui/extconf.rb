=begin
extconf.rb for Ruby/BonoboUI extention library
=end

$LOAD_PATH.unshift File.expand_path(File.dirname(__FILE__) + '/../glib/src/lib')

require 'mkmf-gnome2'

#
# detect BonoboUI configurations
#

(PKGConfig.have_package('libgnomeui-2.0') and 
 PKGConfig.have_package('libbonoboui-2.0')) or exit
check_win32

top = File.expand_path(File.dirname(__FILE__) + '/..') # XXX
$CFLAGS += " " + ['glib/src', 'gtk/src', 'gnome/src'].map{|d|
  "-I" + File.join(top, d)
}.join(" ")

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

begin
  srcdir = File.dirname($0) == "." ? "." :
    File.expand_path(File.dirname($0) + "/src")

  Dir.mkdir "src" unless FileTest.exist? "src"
  Dir.chdir "src"

  begin
    File.delete("rbbonobouiinits.c") if FileTest.exist?("rbbonobouiinits.c")

    obj_ext = ".#{$OBJEXT}"

    $libs = $libs.split(/\s/).uniq.join(' ')
    $source_files = Dir.glob("#{srcdir}/*.c").map{|fname|
      fname[0, srcdir.length+1] = ''
      fname
    }
    $objs = $source_files.collect do |item|
      item.gsub(/\.c$/, obj_ext)
    end
    $objs << "rbbonobouiinits#{obj_ext}"

    #
    # create Makefiles
    #
    create_makefile("bonoboui2", srcdir)

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
  ensure
    Dir.chdir ".."
  end

  create_top_makefile

rescue Interrupt
#  if $DEBUG
    print "  [error] " + $!.to_s + "\n"
#  end
end
