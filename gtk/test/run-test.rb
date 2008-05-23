#!/usr/bin/env ruby

ruby_gnome2_base = File.join(File.dirname(__FILE__), "..", "..")
ruby_gnome2_base = File.expand_path(ruby_gnome2_base)

glib_base = File.join(ruby_gnome2_base, "glib")
atk_base = File.join(ruby_gnome2_base, "atk")
pango_base = File.join(ruby_gnome2_base, "pango")
gdk_pixbuf_base = File.join(ruby_gnome2_base, "gdkpixbuf")
gtk_base = File.join(ruby_gnome2_base, "gtk")
test_unit_ext_dir = File.join(glib_base, "test-unit-ext", "lib")

[glib_base, atk_base, pango_base, gdk_pixbuf_base, gtk_base].each do |target|
  if system("which make > /dev/null")
    `make -C #{target.dump} > /dev/null` or exit(1)
  end
  $LOAD_PATH.unshift(File.join(target, "src"))
  $LOAD_PATH.unshift(File.join(target, "src", "lib"))
  $LOAD_PATH.unshift(File.join(target))
  $LOAD_PATH.unshift(File.join(target, "lib"))
end

$LOAD_PATH.unshift(test_unit_ext_dir)
require 'test-unit-ext'

$LOAD_PATH.unshift(File.join(gtk_base, "test"))
require 'gtk-test-utils'

require 'pp'
require 'gtk2'

Dir.glob(File.join(gtk_base, "test", "test_*.rb")) do |file|
  require file.gsub(/\.rb$/, '')
end
