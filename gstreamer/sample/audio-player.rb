#!/usr/bin/env ruby

require 'gst'

if ARGV.length != 1
    puts "Usage: #{__FILE__} mp3-file"
    exit
end

file = ARGV.first

pipeline = Gst::Pipeline.new
filesrc = Gst::ElementFactory.make("filesrc")
filesrc.location = file
pipeline.add(filesrc)

autobin = Gst::Bin.new

typefind = Gst::ElementFactory.make("typefind")
typefind.signal_connect("have_type") do |typefind, caps|
    $stderr.puts("GstPipeline: play have type")
    pipeline.pause
    autobin.remove(typefind)
  audio_sink = Gst::ElementFactory.make("gconfaudiosink", "audio-sink")
  p audio_sink
    osssink = Gst::ElementFactory.make("osssink")
    autoplug = Gst::AutoplugFactory.make("staticrender")
    unless element = autoplug.to_renderers(caps, osssink)
        puts "Could not autoplug, no suitable codecs found..."
        exit 1
    end
    element.name = "new element"
    autobin.add(element)
    pipeline.play
end
# autobin.add(filesrc, typefind)

pipeline.add(typefind)
pipeline.link(filesrc)
pipeline.link(typefind)

pipeline.play
loop = GLib::MainLoop.new(nil, false)
loop.run
begin
    while pipeline.iterate do; p 1;  end
rescue Interrupt
# ensure
#     # XXX stop the pipeline
#     exit!   # exits all running children (needed)
end

