=begin
extconf.rb for Ruby/Pango extention library
=end

$LOAD_PATH.unshift File.expand_path(File.dirname(__FILE__) + '/../glib/src/lib')
require 'mkmf-gnome2'

PKGConfig.have_package('pango') or exit 1
check_win32

top = File.expand_path(File.dirname(__FILE__) + '/..') # XXX
$CFLAGS += " " + ['glib/src'].map{|d|
  "-I" + File.join(top, d)
}.join(" ")

if /cygwin|mingw/ =~ RUBY_PLATFORM
  top = "../.."
  [
    ["glib/src", "ruby-glib2"],
  ].each{|d,l|
    $libs << " -l#{l}"
    $LDFLAGS << " -L#{top}/#{d}"
  }
end
set_output_lib('libruby-pango.a')

$distcleanfiles << "rbpangoinits.c" if $distcleanfiles

begin
  srcdir = File.dirname($0) == "." ? "." :
    File.expand_path(File.dirname($0) + "/src")

  Dir.mkdir('src') unless File.exist? 'src'
  Dir.chdir "src"

  File.delete("rbpangoinits.c") if FileTest.exist?("rbpangoinits.c")
  system("ruby #{srcdir}/makeinits.rb #{srcdir}/*.c > rbpangoinits.c")

  $objs = []
  Dir.glob("#{srcdir}/*.c") do |f|
    f = File.basename(f)
    f.sub!(/.c$/, ".o")
    add_obj(f)
  end
  add_obj("rbpangoinits.o")

  $defs << "-DRUBY_PANGO_COMPILATION"
  create_makefile("pango", srcdir)
ensure
  Dir.chdir('..')
end

create_top_makefile

