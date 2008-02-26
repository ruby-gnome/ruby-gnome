#!/usr/bin/env ruby

if ARGV.size != 1
  puts "Usage: #{$0} ogg-audio-file"
  exit
end

require 'gst'

loop = GLib::MainLoop.new(nil, false)

pipeline = Gst::Pipeline.new("ogg-audio-player")
source = Gst::ElementFactory.make("filesrc", "file-source")
parser = Gst::ElementFactory.make("oggdemux", "ogg-parser")
decoder = Gst::ElementFactory.make("vorbisdec", "vorbis-decoder")
converter = Gst::ElementFactory.make("audioconvert", "converter")
sink = Gst::ElementFactory.make("alsasink", "alsa-output")

source.location = ARGV.shift

bus = pipeline.bus
bus.add_watch do |bus, message|
  case message.type
  when Gst::Message::EOS
    loop.quit
  when Gst::Message::ERROR
    p message.parse
    loop.quit
  end
  true
end


pipeline.add(source, parser, decoder, converter, sink)
source >> parser
decoder >> converter >> sink

parser.signal_connect("pad-added") do |element, pad|
  sink_pad = decoder.get_pad("sink")
  pad.link(sink_pad)
end

puts "Setting to PLAYING"
pipeline.play
puts "Running"
begin
  loop.run
  print "Returned, "
rescue Interrupt
  print "Interrupted, "
ensure
  puts "stopping playback"
  pipeline.stop
end
