#!/usr/bin/env ruby

base = File.expand_path(File.join(File.dirname(__FILE__)))
top = File.expand_path(File.join(base, ".."))

$LOAD_PATH.unshift(top)
require 'test/glib-test-init'

if system("which make > /dev/null")
  system("cd #{top.dump} && make > /dev/null") or exit(1)
end

$LOAD_PATH.unshift(File.join(top, "ext", "glib2"))
$LOAD_PATH.unshift(File.join(top, "lib"))

$LOAD_PATH.unshift(base)
require 'glib-test-utils'

require 'glib2'

exit Test::Unit::AutoRunner.run(true, base)
