#!/usr/bin/env ruby

ruby_gnome2_base = File.join(File.dirname(__FILE__), "..", "..")
ruby_gnome2_base = File.expand_path(ruby_gnome2_base)

glib_base = File.join(ruby_gnome2_base, "glib2")
atk_base = File.join(ruby_gnome2_base, "atk")
pango_base = File.join(ruby_gnome2_base, "pango")
gdk_pixbuf_base = File.join(ruby_gnome2_base, "gdk_pixbuf2")
gtk_base = File.join(ruby_gnome2_base, "gtk2")

[[glib_base, "glib2"],
 [atk_base, "atk"],
 [pango_base, "pango"],
 [gdk_pixbuf_base, "gdk_pixbuf2"],
 [gtk_base, "gtk2"]].each do |target, module_name|
  if system("which make > /dev/null")
    `make -C #{target.dump} > /dev/null` or exit(false)
  end
  $LOAD_PATH.unshift(File.join(target, "ext", module_name))
  $LOAD_PATH.unshift(File.join(target, "lib"))
end

$LOAD_PATH.unshift(File.join(glib_base, "test"))
require 'glib-test-init'

$LOAD_PATH.unshift(File.join(gtk_base, "test"))
require 'gtk-test-utils'

require 'gtk2'

exit Test::Unit::AutoRunner.run(true)
