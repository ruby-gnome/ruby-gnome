#!/usr/bin/env ruby
# -*- coding: utf-8 -*-
#
# This sample code is a port of
# gstreamer/tests/examples/typefind/typefind.c. It is licensed
# under the terms of the GNU Library General Public License, version
# 2 or (at your option) later.
#
# Here is the original copyright header:
#   Copyright (C) <2005> Stefan Kost
#   Copyright (C) <2006> Tim-Philipp Müller
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
require "rexml/document"

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

if ARGV.length < 1
  puts "usage: #{$0} <filename>"
  exit(false)
end

# create a new pipeline to hold the elements
pipeline = Gst::Pipeline.new("pipeline")
raise "'pipeline' gstreamer plugin missing" if pipeline.nil?

# create a file reader
filesrc = Gst::ElementFactory.make("filesrc", "file_source")
raise "'filesrc' gstreamer plugin missing" if filesrc.nil?
filesrc.location = ARGV.first

typefind = Gst::ElementFactory.make("typefind", "typefind")
raise "'typefind' gstreamer plugin missing" if typefind.nil?

sink = Gst::ElementFactory.make("fakesink", "sink")
raise "'fakesink' gstreamer plugin missing" if sink.nil?

# add objects to the main pipeline
pipeline << filesrc << typefind << sink

typefind.signal_connect("have-type") do |element, probability, caps|
  caps_str = caps.to_s
  xml = REXML::Document.new
  xml << REXML::XMLDecl.new
  capabilities = xml.add_element("Capabilities")
  cap1 = capabilities.add_element("Cap1")
  cap1.add_text(caps_str)
  xml.write($stdout, 2)
  puts
end

filesrc >> typefind >> sink

# start playing
pipeline.play

# Run event loop listening for bus messages until EOS or ERROR
event_loop(pipeline)

# stop the bin
pipeline.stop
