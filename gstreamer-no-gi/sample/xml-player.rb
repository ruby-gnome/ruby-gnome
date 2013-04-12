#!/usr/bin/env ruby

### TODO: I want to set link in build_pipeline.

unless ARGV.size == 1
  puts "Usage: #{$0} [file]"
  exit 1
end

require 'gst'

XML_FILE = "/tmp/pipeline.xml"
PIPELINE_NAME = "our-pipeline"

def build_pipeline(file)
  source = Gst::ElementFactory.make("filesrc")
  decoder = Gst::ElementFactory.make("decodebin")
  audio_converter = Gst::ElementFactory.make("audioconvert")
  audio_resample = Gst::ElementFactory.make("audioresample")
  audio_sink = Gst::ElementFactory.make("autoaudiosink")
  source.location = file
  pipeline = Gst::Pipeline.new(PIPELINE_NAME)
  pipeline.add(source, decoder, audio_converter, audio_resample, audio_sink)
  pipeline
end

def write_pipeline(pipeline, file)
  Gst::XML.write_file(pipeline, file)
end

def read_pipeline(file)
  xml = Gst::XML.new
  xml.parse_file(file)
  pipeline = xml.get_element(PIPELINE_NAME)
  children = pipeline.children
  source, decoder, audio_converter, audio_resample, audio_sink = children
  source >> decoder
  audio_converter >> audio_resample >> audio_sink
  decoder.signal_connect("new-decoded-pad") do |element, pad|
    sink_pad = audio_converter.get_pad("sink")
    pad.link(sink_pad)
  end
  pipeline
end

def play_pipeline(pipeline)
  pipeline.play
  loop = GLib::MainLoop.new(nil, false)
  begin
    loop.run
  rescue Interrupt
  ensure
    pipeline.stop
  end
end

pipeline = build_pipeline(ARGV.first)
write_pipeline(pipeline, XML_FILE)
pipeline = read_pipeline(XML_FILE)
play_pipeline(pipeline)
