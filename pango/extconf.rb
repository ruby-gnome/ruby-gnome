=begin
extconf.rb for pango extention library
=end

$LOAD_PATH.unshift File.expand_path(File.dirname(__FILE__) + '/..')
require 'mkmf'
require 'mkmf-gnome2'

#
# detect configurations
#

PKGConfig.have_package('pango') or exit
check_win32

STDOUT.print("checking for new allocation framework... ") # for ruby-1.7
if Object.respond_to? :allocate
  STDOUT.print "yes\n"
  $defs << "-DHAVE_OBJECT_ALLOCATE"
else
  STDOUT.print "no\n"
end

top = File.expand_path(File.dirname(__FILE__) + '/..') # XXX
$CFLAGS += " " + ['glib/src'].map{|d|
  "-I" + File.join(top, d)
}.join(" ")

have_func("rb_define_alloc_func") # for ruby-1.8


set_output_lib('libruby-pango.a')
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
    File.delete("rbpangoinits.c") if FileTest.exist?("rbpangoinits.c")

    obj_ext = ".#{$OBJEXT}"

    $libs = $libs.split(/\s/).uniq.join(' ')
    $source_files = Dir.glob("#{srcdir}/*.c").map{|fname|
      fname[0, srcdir.length+1] = ''
      fname
    }
    $objs = $source_files.collect do |item|
      item.gsub(/\.c$/, obj_ext)
    end
    $objs << "rbpangoinits.o"

    #
    # create Makefiles
    #
    $defs << "-DRUBY_PANGO_COMPILATION"
    create_makefile("pango", srcdir)
    raise Interrupt if not FileTest.exist? "Makefile"

    mfile = File.open("Makefile", "a")
    $source_files.each do |e|
      mfile.print "#{e.gsub(/\.c$/, obj_ext)}: #{e} rbpango.h \n"
    end
    mfile.print "\
rbpangoinits.c:;    $(RUBY) $(srcdir)/makeinits.rb $(srcdir)/*.c > $@
allclean: clean
	rm -rf *.a
"
    mfile.close
  ensure
    Dir.chdir ".."
  end

  create_top_makefile()
rescue Interrupt
  print "  [error] " + $!.to_s + "\n"
end


