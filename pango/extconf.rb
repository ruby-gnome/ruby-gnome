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

have_func("rb_define_alloc_func") # for ruby-1.8

$CFLAGS = format('-I%s ', File.expand_path(File.dirname(__FILE__) + '/../glib/src')) + $CFLAGS

mdir = $mdir
begin
  $mdir = "pango/src"
  Dir.chdir "src"
  File.delete("rbpangoinits.c") if FileTest.exist?("rbpangoinits.c")

  obj_ext = ".#{$OBJEXT}"

  $libs = $libs.split(/\s/).uniq.join(' ')
  $source_files = Dir.glob('*.c')
  $objs = $source_files.collect do |item|
	item.gsub(/\.c$/, obj_ext)
  end
  $objs << "rbpangoinits.o"

  $defs << "-DRUBY_PANGO_COMPILATION"

  #
  # create Makefiles
  #
  srcdir = File.dirname($0) == "." ? "." : "../src"
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
