=begin
extconf.rb for pango extention library
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
# detect configurations
#

begin
  config_cmd = with_config("pkg-config", "pkg-config pango")
  config_libs = "--libs"
  config_cflags = "--cflags"
  config_library = ""
  version = `#{config_cmd} --version`
  if not version.chomp.empty? then
    $libs += ' ' + `#{config_cmd} #{config_libs} #{config_library}`.chomp
    $CFLAGS += " -I../../glib/src " +
      `#{config_cmd} #{config_cflags} #{config_library}`.chomp
  else
    raise "Can't find a config command"
  end
rescue
  prefix = '/usr/local'
  $LDFLAGS = '-L/usr/X11R6/lib -L#{prefix}/lib'
  $CFLAGS = ' -I/usr/include/pango-1.0 -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include '
  $libs = ' -lpango-1.0 -lgobject-2.0 -lgmodule-2.0 -ldl -lglib-2.0 '
end

STDOUT.print("checking for GCC... ")
STDOUT.flush
if macro_defined?("__GNUC__", "")
  STDOUT.print "yes\n"
  $CFLAGS += ' -Wall' 
  is_gcc = true
else
  STDOUT.print "no\n"
  is_gcc = false
end

STDOUT.print("checking for new allocation framework... ") # for ruby-1.7
if Object.respond_to? :allocate
  STDOUT.print "yes\n"
  $defs << "-DHAVE_OBJECT_ALLOCATE"
else
  STDOUT.print "no\n"
end

have_func("rb_define_alloc_func") # for ruby-1.8

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
