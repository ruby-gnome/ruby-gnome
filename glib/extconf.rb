require 'mkmf'

pkgconfig = with_config('pkg-config', 'pkg-config')
pkgname   = 'gobject-2.0'

pkgconfig_opt = ''
if /mswin32/ =~ RUBY_PLATFORM and /^cl\b/ =~ Config::CONFIG['CC']
  pkgconfig_opt += ' --msvc-syntax'
end


unless system(pkgconfig, '--exists', pkgname)
  STDERR.printf("%s doesn't exist\n", pkgname)
  exit
end

$libs   += ' ' + `#{pkgconfig} #{pkgconfig_opt} #{pkgname} --libs`.chomp
$CFLAGS += ' ' + `#{pkgconfig} #{pkgconfig_opt} #{pkgname} --cflags`.chomp
$CFLAGS += ' -g'

if /mswin32|mingw|bcc/ =~ RUBY_PLATFORM and /gcc/ =~ Config::CONFIG['CC']
  $CFLAGS += ' -fnative-struct'
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
  create_makefile("glib2", src_dir)
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
		@-rm -f glib2.lib *~
"
  else
    mfile.print "\

all:
		@cd src; make all

install:;	@cd src; make install
site-install:;	@cd src; make site-install
clean:
		@cd src; make clean
		@rm -f core glib2.a *~
distclean:	clean
		@cd src; make distclean
		@rm -f Makefile extconf.h conftest.*
		@rm -f core *~ mkmf.log
"
  end
}
