#!/usr/bin/env ruby

require 'gst'

def type_find(file)
  pipeline = Gst::Pipeline.new("pipeline")

  source = Gst::ElementFactory.make("filesrc", "source")
  type_find = Gst::ElementFactory.make("typefind", "type-find")
  sink = Gst::ElementFactory.make("fakesink", "sink")

  pipeline.add(source, type_find, sink)
  source >> type_find >> sink

  found_caps = nil
  type_find.signal_connect("have-type") do |element, probability, caps|
    found_caps = caps
  end

  loop = GLib::MainLoop.new
  source.location = file
  bus = pipeline.bus
  bus.add_watch do |bus, message|
    case message.type
    when Gst::Message::STATE_CHANGED, Gst::Message::EOS
      loop.quit
    when Gst::Message::ERROR
      p message.parse
      loop.quit
    end
    true
  end
  pipeline.pause
  loop.run
  pipeline.stop

  if found_caps
    puts("#{file} - #{found_caps}")
  else
    puts("#{file} - No type found")
  end
end

ARGV.each do |file|
  type_find(file)
end
