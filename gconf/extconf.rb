require "mkmf"

# TODO: do we need this? hopefully glib2 is ahead of us in build-order?
begin
  require 'glib2'
rescue LoadError
  raise "Install glib2 before gtk2."
end

unless defined? macro_defined?
  def macro_defined?(macro, src, opt="")
    try_cpp(src + <<EOP, opt)
#ifndef #{macro}
# error
#endif
EOP
  end
end


pkgconfig = with_config('pkg-config', 'pkg-config')
pkgname   = 'gconf-2.0'

pkgconfig_opt = ''
if /mswin32/ =~ RUBY_PLATFORM and /^cl\b/ =~ Config::CONFIG['CC']
  pkgconfig_opt += ' --msvc-syntax'
end


unless system("#{pkgconfig} #{pkgconfig_opt} --exists #{pkgname}")
  STDERR.printf("%s doesn't exist\n", pkgname)
  exit
end

$libs   += ' ' + `#{pkgconfig} #{pkgconfig_opt} #{pkgname} --libs`.chomp
$CFLAGS += ' ' + `#{pkgconfig} #{pkgconfig_opt} #{pkgname} --cflags`.chomp

STDOUT.print("checking for GCC... ")
if /gcc/ =~ Config::CONFIG['CC']
  STDOUT.print "yes\n"
  $CFLAGS += ' -Wall' 
  is_gcc = true
else
  STDOUT.print "no\n"
  is_gcc = false
end

$CFLAGS = format('-I%s ', File.expand_path(File.dirname(__FILE__) + '/../glib/src')) + $CFLAGS

STDOUT.print("checking for G_OS_WIN32... ")
STDOUT.flush
if macro_defined?('G_OS_WIN32', "#include <glibconfig.h>\n")
  STDOUT.print "yes\n"
  $CFLAGS += ' -fnative-struct' if is_gcc
else
  STDOUT.print "no\n"
end

STDOUT.print("checking for new allocation framework... ") # for ruby-1.7
if Object.respond_to? :allocate
  STDOUT.print "yes\n"
  $defs << "-DHAVE_OBJECT_ALLOCATE"
else
  STDOUT.print "no\n"
end


src_dir = File.expand_path(File.join(File.dirname(__FILE__), 'src'))

Dir.mkdir('src') unless File.exist? 'src'
Dir.chdir "src"
begin
  create_makefile("gconf2", src_dir)
ensure
  Dir.chdir('..')
end

File.open("Makefile", "w"){|mfile|
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
		@nmake -nologo clean
		@cd ..
		@-rm -f Makefile extconf.h conftest.*
		@-rm -f gconf2.lib *~
"
  else
    mfile.print "\

all:
		@cd src; make all

install:;	@cd src; make install
site-install:;	@cd src; make site-install
clean:
		@cd src; make clean
		@rm -f core gconf2.a *~
distclean:	clean
		@cd src; make distclean
		@rm -f Makefile extconf.h conftest.*
		@rm -f core *~ mkmf.log
"
  end
}
