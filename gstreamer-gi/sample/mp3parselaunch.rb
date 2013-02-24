#!/usr/bin/env ruby
# -*- coding: utf-8 -*-
#
# This sample code is a port of
# gstreamer/tests/examples/launch/mp3parselaunch.c. It is licensed
# under the terms of the GNU Library General Public License, version
# 2 or (at your option) later.
#
# Copyright (C) 2013  Ruby-GNOME2 Project Team
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

def event_loop(pipe)
  running = true
  bus = pipe.bus

  while running do
    message = bus.poll(Gst::MessageType::ANY, Gst::CLOCK_TIME_NONE)
    raise "message nil" if message.nil?

    case message.type
    when Gst::MessageType::EOS
      running = false
    when Gst::MessageType::WARNING
      warning, debug = message.parse_warning
      puts "Debugging info: #{debug || 'none'}"
      puts "Warning: #{warning.message}"
    when Gst::MessageType::ERROR
      error, debug = message.parse_error
      puts "Debugging info: #{debug || 'none'}"
      puts "Error: #{error.message}"
      running = false
    end
  end
end

Gst.init

if ARGV.length < 1
  puts "usage: #{$0} <mp3 file>"
  exit(false)
end

bin, error = Gst::parse_launch("filesrc name=my_filesrc ! mad ! autoaudiosink")
if bin.nil?
  $stderr.puts "Parse error: #{error.message}"
  exit(false)
end

filesrc = bin.get_by_name("my_filesrc")
filesrc.location = ARGV[0]

# start playing
bin.play

# Run event loop listening for bus messages until EOS or ERROR
event_loop(bin)

# stop the bin
bin.stop
