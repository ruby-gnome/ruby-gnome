#!/usr/bin/env ruby

$VERBOSE = true

ruby_gnome2_base = File.join(File.dirname(__FILE__), "..", "..")
ruby_gnome2_base = File.expand_path(ruby_gnome2_base)

glib_base = File.join(ruby_gnome2_base, "glib2")
pango_base = File.join(ruby_gnome2_base, "pango")

$LOAD_PATH.unshift(File.join(glib_base, "test"))
require 'glib-test-init'

[[glib_base, "glib2"],
 [pango_base, "pango"]].each do |target, module_name|
  if system("which make > /dev/null")
    `make -C #{target.dump} > /dev/null` or exit(1)
  end
  $LOAD_PATH.unshift(File.join(target, "ext", module_name))
  $LOAD_PATH.unshift(File.join(target, "lib"))
end

$LOAD_PATH.unshift(File.join(pango_base, "test"))
require 'pango-test-utils'

require 'pango'

exit Test::Unit::AutoRunner.run(true)
