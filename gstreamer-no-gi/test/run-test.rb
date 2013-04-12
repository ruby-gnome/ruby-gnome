#!/usr/bin/env ruby

# ENV["GST_DEBUG"] = "GST_BUFFER:5"

base_dir = File.expand_path(File.join(File.dirname(__FILE__), ".."))

if system("which make > /dev/null")
  system("cd #{base_dir.dump} && make > /dev/null") or exit(1)
end

glib_dir = File.expand_path(File.join(base_dir, "..", "glib2"))
$LOAD_PATH.unshift(File.join(glib_dir, "test"))
require 'glib-test-init'

$LOAD_PATH.unshift(File.join(glib_dir, "ext", "glib2"))
$LOAD_PATH.unshift(File.join(glib_dir, "lib"))

$LOAD_PATH.unshift(File.join(base_dir, "ext", "gstreamer"))
$LOAD_PATH.unshift(File.join(base_dir, "lib"))
require "gst"

$LOAD_PATH.unshift(File.join(base_dir, "test"))
require 'gst-test-utils'

exit Test::Unit::AutoRunner.run(true)
