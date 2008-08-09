#!/usr/bin/env ruby

ruby_gnome2_base = File.join(File.dirname(__FILE__), "..", "..")
ruby_gnome2_base = File.expand_path(ruby_gnome2_base)

glib_base = File.join(ruby_gnome2_base, "glib")
pango_base = File.join(ruby_gnome2_base, "pango")

$LOAD_PATH.unshift(glib_base)
require 'test/glib-test-init'

[glib_base, pango_base].each do |target|
  if system("which make > /dev/null")
    `make -C #{target.dump} > /dev/null` or exit(1)
  end
  $LOAD_PATH.unshift(File.join(target, "src"))
  $LOAD_PATH.unshift(File.join(target, "src", "lib"))
end

$LOAD_PATH.unshift(File.join(pango_base, "test"))
require 'pango-test-utils'

require 'pango'

exit Test::Unit::AutoRunner.run(true)
