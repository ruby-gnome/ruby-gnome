=begin
extconf.rb for Ruby/GdkPixbuf extention library
=end

$LOAD_PATH.unshift File.expand_path(File.dirname(__FILE__) + '/../glib/src/lib')
require 'mkmf-gnome2'

PKGConfig.have_package('gdk-pixbuf-2.0') or exit
check_win32

top = File.expand_path(File.dirname(__FILE__) + '/..') # XXX
$CFLAGS += " " + ['glib/src'].map{|d|
  "-I" + File.join(top, d)
}.join(" ")

if have_func("g_print") && have_func("gdk_pixbuf_new")
  # tml's libgdk_pixbuf-2.0-0.dll doesn't export gdk_pixbuf_version.
  have_func('gdk_pixbuf_version') # XXX

  if /cygwin|mingw/ =~ RUBY_PLATFORM
    top = '..'
    [
      ["glib/src", "ruby-glib2"],
    ].each{|d,l|
      $LDFLAGS << " -L#{top}/#{d}"
      $libs << " -l#{l}"
    }
  end

  create_makefile('gdk_pixbuf2')
end
