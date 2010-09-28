#!/usr/bin/env ruby

base_dir = File.expand_path(File.join(File.dirname(__FILE__), ".."))
glib_dir = File.expand_path(File.join(base_dir, "..", "glib2"))

$LOAD_PATH.unshift(File.join(glib_dir, "ext", "glib2"))
$LOAD_PATH.unshift(File.join(glib_dir, "lib"))

$LOAD_PATH.unshift(File.join(glib_dir, "test"))
require 'glib-test-init'

if system("which make > /dev/null")
  system("cd #{base_dir.dump} && make > /dev/null") or exit(1)
end

$LOAD_PATH.unshift(File.join(base_dir, "src"))
$LOAD_PATH.unshift(File.join(base_dir, "src", "lib"))

exit Test::Unit::AutoRunner.run(true)
