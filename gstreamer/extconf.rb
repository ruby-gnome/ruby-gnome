=begin
extconf.rb for Ruby/GStreamer extention library
=end

$LOAD_PATH.unshift File.expand_path(File.dirname(__FILE__) + '/../glib/src/lib')
require 'mkmf-gnome2'

pkgname   = 'gstreamer-0.8'

PKGConfig.have_package(pkgname) or exit 1

check_win32

top = File.expand_path(File.dirname(__FILE__) + '/..') # XXX
$CFLAGS += " " + ['glib/src'].map{|d|
  "-I" + File.join(top, d)
}.join(" ")

srcdir = File.dirname($0) == "." ? "." :
  File.expand_path(File.dirname($0) + "/src")

if have_library("gstmedia-info-0.8", "gst_media_info_read")
	$CFLAGS += " -DHAVE_MEDIA_INFO"
end

if /cygwin|mingw/ =~ RUBY_PLATFORM
  top = "../.."
  [
    ["glib/src", "ruby-glib2"],
  ].each{|d,l|
    $LDFLAGS << " -L#{top}/#{d}"
    $libs << " -l#{l}"
  }
end

Dir.mkdir('src') unless File.exist? 'src'
Dir.chdir "src"
begin
  create_makefile("gst", srcdir)
ensure
  Dir.chdir('..')
end

create_top_makefile
