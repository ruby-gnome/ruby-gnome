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

  source.location = file
  pipeline.pause
  while pipeline.state != Gst::State::PAUSED # FIXME
  end
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
