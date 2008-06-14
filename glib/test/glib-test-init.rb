$VERBOSE = true

glib_top = File.expand_path(File.join(File.dirname(__FILE__), ".."))

$LOAD_PATH.unshift(File.join(glib_top, "test-unit", "lib"))
require 'test/unit'
