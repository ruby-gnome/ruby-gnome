=begin
extconf.rb for Ruby/Libglade2 extention library
=end

$LOAD_PATH.unshift File.expand_path(File.dirname(__FILE__) + '/..')
require "mkmf"
require 'mkmf-gnome2'

PKGConfig.have_package('libglade-2.0') or exit
check_win32

top = File.expand_path(File.dirname(__FILE__) + '/..') # XXX
$CFLAGS += " " + ['glib/src', 'gtk/src'].map{|d|
  "-I" + File.join(top, d)
}.join(" ")

if /cygwin|mingw/ =~ RUBY_PLATFORM
  top = ".."
  [
    ["glib/src", "ruby-glib2"],
    ["gtk/src", "ruby-gtk2"],
  ].each{|d,l|
    $LDFLAGS << " -L#{top}/#{d}"
    $libs << " -l#{l}"
  }
end

create_makefile("libglade2")  
