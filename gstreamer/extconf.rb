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

have_header("assert.h")

if have_library("gstmedia-info-0.6", "gst_media_info_read")
	$CFLAGS += " -DHAVE_MEDIA_INFO"
end

Dir.mkdir('src') unless File.exist? 'src'
Dir.chdir "src"
begin
  create_makefile("gst", srcdir)
ensure
  Dir.chdir('..')
end

create_top_makefile
