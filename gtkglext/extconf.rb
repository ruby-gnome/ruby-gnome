=begin
extconf.rb for Ruby/GtkGLExt extention library
=end

$LOAD_PATH.unshift File.expand_path(File.dirname(__FILE__) + '/../glib/src/lib')
require 'mkmf-gnome2'

pkgname = 'gtkglext-1.0'
PKGConfig.have_package(pkgname) or exit 1

top = File.expand_path(File.dirname(__FILE__) + '/..') # XXX
$CFLAGS += " " + ['glib/src', 'gtk/src'].map{|d|
  "-I" + File.join(top, d)
}.join(" ")

srcdir = File.dirname($0) == "." ? "." :
  File.expand_path(File.dirname($0) + "/src")

Dir.mkdir('src') unless File.exist? 'src'
Dir.chdir "src"
begin
  create_makefile("gtkglext", srcdir)
ensure
  Dir.chdir('..')
end

create_top_makefile
