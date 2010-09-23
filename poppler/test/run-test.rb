#!/usr/bin/env ruby

base_dir = File.expand_path(File.join(File.dirname(__FILE__), ".."))

if system("which make > /dev/null")
  system("cd #{base_dir.dump} && make > /dev/null") or exit(1)
end

glib_dir = File.expand_path(File.join(base_dir, "..", "glib"))
gtk_dir = File.expand_path(File.join(base_dir, "..", "gtk"))

$LOAD_PATH.unshift(File.join(glib_dir, "test"))
require 'glib-test-init'

[[gtk_dir, "gtk2"],
 [glib_dir, "glib2"],
 [base_dir, "poppler"]].each do |dir, module_name|
  $LOAD_PATH.unshift(File.join(dir, "ext", module_name))
  $LOAD_PATH.unshift(File.join(dir, "lib"))
end
require "poppler"

$LOAD_PATH.unshift(File.join(base_dir, "test"))
require 'poppler-test-utils'
class Test::Unit::TestCase
  include PopplerTestUtils
end

exit Test::Unit::AutoRunner.run(true)
