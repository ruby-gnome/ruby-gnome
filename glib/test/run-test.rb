#!/usr/bin/env ruby

$VERBOSE = true

base = File.expand_path(File.dirname(__FILE__))
top = File.expand_path(File.join(base, ".."))

if system("which make > /dev/null")
  system("cd #{top.dump} && make > /dev/null") or exit(1)
end

$LOAD_PATH.unshift(File.join(top, "src"))
$LOAD_PATH.unshift(File.join(top, "src", "lib"))
$LOAD_PATH.unshift(base)

$LOAD_PATH.unshift(File.join(top, "test-unit-ext", "lib"))
require 'test-unit-ext'

$LOAD_PATH.unshift(File.join(base, "test"))
require 'glib-test-utils'

require 'glib2'

exit Test::Unit::AutoRunner.run(true, base)
