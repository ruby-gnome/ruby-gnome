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

begin
  File.delete("src/rbpangoinits.c") if FileTest.exist?("src/rbpangoinits.c")

  Dir.chdir "src"
  system("ruby makeinits.rb *.c > rbpangoinits.c")
  $objs = []
  Dir.glob("*.c") do |f|
    f = File.basename(f)
    f.sub!(/.c$/, ".o")
    $objs.push f
  end
  $defs << "-DRUBY_PANGO_COMPILATION"
  create_makefile("pango", File.expand_path(File.dirname($0) + "/src"))
ensure
  Dir.chdir('..')
end

create_top_makefile

