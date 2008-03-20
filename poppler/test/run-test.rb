#!/usr/bin/env ruby

$VERBOSE = true

base_dir = File.expand_path(File.join(File.dirname(__FILE__), ".."))

if system("make --version > /dev/null")
  system("cd #{base_dir.dump} && make > /dev/null") or exit(1)
end

glib_dir = File.expand_path(File.join(base_dir, "..", "glib"))
gtk_dir = File.expand_path(File.join(base_dir, "..", "gtk"))
test_unit_ext_dir = File.join(glib_dir, "test-unit-ext", "lib")

[gtk_dir, glib_dir, base_dir].each do |dir|
  $LOAD_PATH.unshift(File.join(dir, "src"))
  $LOAD_PATH.unshift(File.join(dir, "src", "lib"))
end
require "poppler"

$LOAD_PATH.unshift(test_unit_ext_dir)
require 'test-unit-ext'

$LOAD_PATH.unshift(File.join(base_dir, "test"))
require 'poppler-test-utils'
class Test::Unit::TestCase
  include PopplerTestUtils
end

Test::Unit::TestSuite.priority_mode = true

exit Test::Unit::AutoRunner.run($0)
