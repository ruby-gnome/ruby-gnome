#!/usr/bin/env ruby

system("make")

$LOAD_PATH.unshift(File.expand_path("./src"))
$LOAD_PATH.unshift(File.expand_path("./src/lib"))

require "test/unit"

Test::Unit::AutoRunner.run(false, File.dirname($0))
