#!/usr/bin/env ruby
# coding: utf-8

require 'gvlc'

puts "Version: #{VLC.version}"
puts "Compiler: #{VLC.compiler}"
puts "Changeset: #{VLC.changeset}"

core = VLC::Core.new

puts ""
puts "Video Filters:"
core.video_filter_list.each do |f|
  puts " * #{f.name} (#{f.longname})"
end

puts ""
puts "Audio Filters:"
core.audio_filter_list.each do |f|
  puts " * #{f.name} (#{f.longname})"
end

puts ""
puts "Audio Outputs:"
core.audio_output_list.each do |o|
  puts " * #{o.name} (#{o.description})"
  core.audio_output_device_count(o.name).times do |i|
    puts "#{' ' * 5}#{core.audio_output_device_id(o.name, i)}(#{core.audio_output_device_longname(o.name, i)})"
  end
end

