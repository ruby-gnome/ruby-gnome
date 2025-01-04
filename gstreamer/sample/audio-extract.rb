#!/usr/bin/env ruby
#
# Copyright (C) 2024  Ruby-GNOME Project Team
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

pipeline = Gst::Pipeline.new("audio-extractor")
src = Gst::ElementFactory.make("autoaudiosrc", nil)
raise "need audiotestsrc from gst-plugins-good" if src.nil?
resample = Gst::ElementFactory.make("audioresample", nil)
raise "need audioresample from gst-plugins-base" if resample.nil?
sink = Gst::ElementFactory.make("appsink", nil)
raise "need appsink from gst-plugins-base" if sink.nil?

# See https://gstreamer.freedesktop.org/documentation/additional/design/mediatype-audio-raw.html
caps = Gst::Caps.new("audio/x-raw")
caps.set_value("format", "F32LE")
caps.set_value("rate", GLib::Value.new(GLib::Type::INT, 16 * 1000))
caps.set_value("channels", GLib::Value.new(GLib::Type::INT, 1))
sink.caps = caps

sink.emit_signals = true
sink.signal_connect(:new_sample) do |_|
  sample = sink.pull_sample
  buffer = sample.buffer
  success, map = buffer.map(:read)
  p map.data
  buffer.unmap(map)
  Gst::FlowReturn::OK
end

pipeline << src << resample << sink
src >> resample >> sink

loop = GLib::MainLoop.new

bus = pipeline.bus
bus.add_watch do |bus, message|
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
