#!/usr/bin/env ruby

$VERBOSE = true

ruby_gnome2_base = File.join(File.dirname(__FILE__), "..", "..")
ruby_gnome2_base = File.expand_path(ruby_gnome2_base)

glib_base = File.join(ruby_gnome2_base, "glib2")
atk_base = File.join(ruby_gnome2_base, "atk")
pango_base = File.join(ruby_gnome2_base, "pango")
gdk_pixbuf_base = File.join(ruby_gnome2_base, "gdkpixbuf")
gtk_base = File.join(ruby_gnome2_base, "gtk2")
gtk_moz_embed_base = File.join(ruby_gnome2_base, "gtkmozembed")

$LOAD_PATH.unshift(glib_base)
require 'test/glib-test-init'

[glib_base, atk_base, pango_base,
 gdk_pixbuf_base, gtk_base, gtk_moz_embed_base].each do |target|
  if system("which make > /dev/null")
    `make -C #{target.dump} > /dev/null` or exit(1)
  end
  $LOAD_PATH.unshift(File.join(target, "src"))
  $LOAD_PATH.unshift(File.join(target, "src", "lib"))
  $LOAD_PATH.unshift(File.join(target))
  $LOAD_PATH.unshift(File.join(target, "lib"))
end

$LOAD_PATH.unshift(File.join(gtk_moz_embed_base, "test"))
require 'gtk-moz-embed-test-utils'

require 'gtkmozembed'

exit Test::Unit::AutoRunner.run(true)
