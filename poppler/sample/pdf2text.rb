#!/usr/bin/env ruby

require "poppler"

if ARGV.size < 1
  puts "usage: #{$0} input.pdf"
  exit(-1)
end

input = ARGV.shift
input_uri = "file://#{File.expand_path(input)}"

doc = Poppler::Document.new(input_uri)
doc.each do |page|
  puts page.text
end
