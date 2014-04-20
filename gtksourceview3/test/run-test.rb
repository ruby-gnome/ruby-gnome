#!/usr/bin/env ruby

ruby_gnome2_base = File.join(File.dirname(__FILE__), "..", "..")
ruby_gnome2_base = File.expand_path(ruby_gnome2_base)

glib_base = File.join(ruby_gnome2_base, "glib2")
atk_base = File.join(ruby_gnome2_base, "atk")
cairo_gobject_base = File.join(ruby_gnome2_base, "cairo-gobject")
pango_base = File.join(ruby_gnome2_base, "pango")
gdk_pixbuf_base = File.join(ruby_gnome2_base, "gdk_pixbuf2")
gobject_introspection_base = File.join(ruby_gnome2_base, "gobject-introspection")
gdk3_base = File.join(ruby_gnome2_base, "gdk3")
gtk3_base = File.join(ruby_gnome2_base, "gtk3")
gtk_source_view3_base = File.join(ruby_gnome2_base, "gtksourceview3")

$LOAD_PATH.unshift(glib_base)
require 'test/glib-test-init'

[glib_base, atk_base, cairo_gobject_base, pango_base, gdk_pixbuf_base,
 gobject_introspection_base, gdk3_base, gtk3_base,
 gtk_source_view3_base].each do |target|
  if system("which make > /dev/null")
    `make -C #{target.dump} > /dev/null` or exit(1)
  end
  $LOAD_PATH.unshift(File.join(target, "ext", File.basename(target)))
  $LOAD_PATH.unshift(File.join(target, "src"))
  $LOAD_PATH.unshift(File.join(target, "src", "lib"))
  $LOAD_PATH.unshift(File.join(target))
  $LOAD_PATH.unshift(File.join(target, "lib"))
end

$LOAD_PATH.unshift(File.join(gtk3_base, "test"))
require 'gtk-test-utils'

require 'gtksourceview3'

exit Test::Unit::AutoRunner.run(true)
