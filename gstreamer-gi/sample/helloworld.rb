#!/usr/bin/env ruby

require "gst"

if ARGV.length < 1
  puts "usage: #{$0} <media file or uri>"
  exit 1
end

Gst.init

playbin = Gst::ElementFactory.make("playbin")
if playbin.nil?
  puts "'playbin' gstreamer plugin missing"
  exit 1
end

# take the commandline argument and ensure that it is a uri
if Gst::valid_uri?(ARGV.first)
  uri = ARGV.first
else
  uri = Gst::filename_to_uri(ARGV.first)
end
playbin.uri = uri

# create and event loop feed gstreamer bus messages to it
loop = GLib::MainLoop.new(nil, false)

bus = playbin.bus
bus.add_watch(GLib::PRIORITY_DEFAULT) do |bus, message|
  case message.type
  when Gst::MessageType::EOS
    puts "End-of-stream"
    loop.quit
  when Gst::MessageType::ERROR
    error, debug = message.parse_error
    puts "Debugging info: #{debug || 'none'}"
    puts "Error: #{error.message}"
    loop.quit
  end
  true
end

# start play back and listed to events
playbin.state = Gst::State::PLAYING
begin
  loop.run
rescue Interrupt
  puts "Interrupt"
rescue => error
  puts "Error: #{error.message}"
ensure
  playbin.state = Gst::State::NULL
end
