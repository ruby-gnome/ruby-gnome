=begin
extconf.rb for Ruby/GStreamer extention library
=end

$LOAD_PATH.unshift File.expand_path(File.dirname(__FILE__) + '/../glib/src/lib')
require 'mkmf-gnome2'

pkgname   = 'gstreamer-0.6'

PKGConfig.have_package(pkgname) or exit

top = File.expand_path(File.dirname(__FILE__) + '/..') # XXX
$CFLAGS += " " + ['glib/src'].map{|d|
  "-I" + File.join(top, d)
}.join(" ")

srcdir = File.dirname($0) == "." ? "." :
  File.expand_path(File.dirname($0) + "/src")

Dir.mkdir('src') unless File.exist? 'src'
Dir.chdir "src"
begin
  create_makefile("gst", srcdir)
ensure
  Dir.chdir('..')
end

have_header("assert.h")

create_top_makefile
