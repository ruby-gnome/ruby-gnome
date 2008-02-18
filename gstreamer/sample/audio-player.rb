#!/usr/bin/env ruby

require 'gst'

if ARGV.size != 1
  puts "Usage: #{$0} audio-file"
  exit
end

file = ARGV.first

pipeline = Gst::Pipeline.new

file_src = Gst::ElementFactory.make("filesrc")
file_src.location = file

decoder = Gst::ElementFactory.make("decodebin")

audio_convert = Gst::ElementFactory.make("audioconvert")

audio_resample = Gst::ElementFactory.make("audioresample")

audio_sink = Gst::ElementFactory.make("autoaudiosink")

pipeline.add(file_src, decoder, audio_convert, audio_resample, audio_sink)
file_src >> decoder
audio_convert >> audio_resample >> audio_sink

decoder.signal_connect("new-decoded-pad") do |element, pad|
  sink_pad = audio_convert.get_pad("sink")
  pad.link(sink_pad)
end

pipeline.play
loop = GLib::MainLoop.new(nil, false)
begin
  loop.run
rescue Interrupt
ensure
  pipeline.stop
end
