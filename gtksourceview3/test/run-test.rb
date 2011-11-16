#!/usr/bin/env ruby

ruby_gnome2_base = File.join(File.dirname(__FILE__), "..", "..")
ruby_gnome2_base = File.expand_path(ruby_gnome2_base)

glib_base = File.join(ruby_gnome2_base, "glib2")
atk_base = File.join(ruby_gnome2_base, "atk")
pango_base = File.join(ruby_gnome2_base, "pango")
gdk_pixbuf_base = File.join(ruby_gnome2_base, "gdk_pixbuf2")
gtk_base = File.join(ruby_gnome2_base, "gtk2")
gtk_source_view2_base = File.join(ruby_gnome2_base, "gtksourceview2")

$LOAD_PATH.unshift(glib_base)
require 'test/glib-test-init'

[glib_base, atk_base, pango_base, gdk_pixbuf_base, gtk_base,
 gtk_source_view2_base].each do |target|
  if system("which make > /dev/null")
    `make -C #{target.dump} > /dev/null` or exit(1)
  end
  $LOAD_PATH.unshift(File.join(target, "ext", File.basename(target)))
  $LOAD_PATH.unshift(File.join(target, "src"))
  $LOAD_PATH.unshift(File.join(target, "src", "lib"))
  $LOAD_PATH.unshift(File.join(target))
  $LOAD_PATH.unshift(File.join(target, "lib"))
end

$LOAD_PATH.unshift(File.join(gtk_base, "test"))
require 'gtk-test-utils'

require 'gtksourceview2'

exit Test::Unit::AutoRunner.run(true)
