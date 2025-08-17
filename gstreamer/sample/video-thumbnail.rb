#!/usr/bin/env ruby
#
# Copyright (C) 2025  Ruby-GNOME Project Team
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

require "gst"

if ARGV.size < 2
  puts "usage: #{$0} <input> <output>"
  exit(false)
end

input = ARGV[0]
output = ARGV[1]

pipeline = Gst::Pipeline.new
filesrc = Gst::ElementFactory.make("filesrc")
filesrc.location = ARGV[0]
decodebin = Gst::ElementFactory.make("decodebin", nil)
raise "need decodebin from gst-plugins-base" if decodebin.nil?
videoconvert = Gst::ElementFactory.make("videoconvert", nil)
raise "need videoconvert from gst-plugins-base" if decodebin.nil?
image_encoder_name = output.split(".").last + "enc"
image_encoder = Gst::ElementFactory.make(image_encoder_name, nil)
raise "need #{iamge_encoder_name}" if image_encoder.nil?
filesink = Gst::ElementFactory.make("filesink", nil)
filesink.location = output

pipeline << filesrc << decodebin << videoconvert << image_encoder << filesink
filesrc >> decodebin
decodebin.signal_connect(:pad_added) do |_, pad|
  sink_pad = videoconvert.get_static_pad("sink")
  pad.link(sink_pad)
end
videoconvert >> image_encoder >> filesink

loop = GLib::MainLoop.new

bus = pipeline.bus
bus.add_watch do |bus, message|
  case message.type
  when Gst::MessageType::EOS
    loop.quit
  when Gst::MessageType::ERROR
    error, debug = message.parse_error
    puts "Debugging info: #{debug || 'none'}"
    puts "Error: #{error.message}"
    loop.quit
  end
  true
end

pipeline.play
begin
  loop.run
rescue Interrupt
  puts "Interrupt"
rescue => error
  puts "Error: #{error.message}"
ensure
  pipeline.stop
end
