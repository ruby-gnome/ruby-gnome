#!/usr/bin/env ruby

$VERBOSE = true

require "test/unit"

base_dir = File.expand_path(File.join(File.dirname(__FILE__), ".."))
$LOAD_PATH.unshift(File.join(base_dir, "src"))
$LOAD_PATH.unshift(File.join(base_dir, "src", "lib"))
require "gst"

test_file = File.join(base_dir, "test", "test_*.rb")
case ARGV.first
when /\A--\z/
  ARGV.shift
when /\A-/, nil;
else
  test_file = ARGV.shift
end

Dir.glob(test_file) do |file|
  require file
end

argv = ARGV.dup
unless argv.find {|opt| /\A(?:-v|--verbose=)/ =~ opt}
  argv << "--verbose=n"
end

args = [File.dirname($0), argv.dup]
if Test::Unit::AutoRunner.respond_to?(:standalone?)
  args.unshift(false)
else
  args.unshift($0)
end
Test::Unit::AutoRunner.run(*args)
