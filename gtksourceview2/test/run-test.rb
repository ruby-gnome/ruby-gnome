#!/usr/bin/env ruby

ruby_gnome2_base = File.join(File.dirname(__FILE__), "..", "..")
ruby_gnome2_base = File.expand_path(ruby_gnome2_base)

glib_base = File.join(ruby_gnome2_base, "glib2")
gobject_introspection_base = File.join(ruby_gnome2_base, "gobject-introspection")
gio_base = File.join(ruby_gnome2_base, "gio2")
atk_base = File.join(ruby_gnome2_base, "atk")
cairo_gobject_base = File.join(ruby_gnome2_base, "cairo-gobject")
pango_base = File.join(ruby_gnome2_base, "pango")
gdk_pixbuf_base = File.join(ruby_gnome2_base, "gdk_pixbuf2")
gtk_base = File.join(ruby_gnome2_base, "gtk2")
gtk_source_view2_base = File.join(ruby_gnome2_base, "gtksourceview2")

$LOAD_PATH.unshift(glib_base)
require 'test/glib-test-init'

[
  [glib_base, "glib2"],
  [gobject_introspection_base, "gobject-introspection"],
  [gio_base, "gio2"],
  [atk_base, "atk"],
  [cairo_gobject_base, "cairo-gobject"],
  [pango_base, "pango"],
  [gdk_pixbuf_base, "gdk_pixbuf2"],
  [gtk_base, "gtk2"],
  [gtk_source_view2_base, "gtksourceview2"],
].each do |target, module_name|
  if File.exist?("#{target}/Makefile") and system("which make > /dev/null")
    `make -C #{target.dump} > /dev/null` or exit(1)
  end
  $LOAD_PATH.unshift(File.join(target, "ext", module_name))
  $LOAD_PATH.unshift(File.join(target, "lib"))
end

$LOAD_PATH.unshift(File.join(gtk_base, "test"))
require 'gtk-test-utils'

require 'gtksourceview2'

exit Test::Unit::AutoRunner.run(true, File.join(gtk_source_view2_base, "test"))
