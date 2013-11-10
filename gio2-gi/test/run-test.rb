#!/usr/bin/env ruby
# -*- coding: utf-8 -*-

ruby_gnome2_base = File.join(File.dirname(__FILE__), "..", "..")
ruby_gnome2_base = File.expand_path(ruby_gnome2_base)

glib_base = File.join(ruby_gnome2_base, "glib2")
gobject_introspection_base = File.join(ruby_gnome2_base, "gobject-introspection")
gio2_base = File.join(ruby_gnome2_base, "gio2-gi")

modules = [
  [glib_base, "glib2"],
  [gobject_introspection_base, "gobject-introspection"],
  [gio2_base, "gio2-gi"],
]

modules.each do |target, module_name|
  if File.exist?("Makefile") and system("which make > /dev/null")
    `make -C #{target.dump} > /dev/null` or exit(false)
  end
  $LOAD_PATH.unshift(File.join(target, "ext", module_name))
  $LOAD_PATH.unshift(File.join(target, "lib"))
end

$LOAD_PATH.unshift(File.join(glib_base, "test"))
require "glib-test-init"

$VERBOSE = false # TODO: remove me

$LOAD_PATH.unshift(File.join(gio2_base, "test"))
require "gio2-gi-test-utils"

require 'gio2'

exit Test::Unit::AutoRunner.run(true)
