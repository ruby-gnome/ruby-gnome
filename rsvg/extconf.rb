=begin
extconf.rb for Ruby/RSVG extention library
=end

$LOAD_PATH.unshift File.expand_path(File.dirname(__FILE__) + '/../glib/src/lib')
require 'mkmf-gnome2'

pkg_infos   = [
  ['librsvg-2.0', []],
]

pkg_infos.each do |name, version|
	PKGConfig.have_package(name, *version) or exit 1
end

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

srcdir = File.dirname($0) == "." ? "." :
  File.expand_path(File.dirname($0) + "/src")

Dir.mkdir('src') unless File.exist? 'src'
Dir.chdir "src"
begin
  create_makefile("rsvg2", srcdir)
ensure
  Dir.chdir('..')
end

create_top_makefile
