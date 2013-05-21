#!/usr/bin/env ruby
# -*- coding: utf-8 -*-
#
# This sample code is a port of
# gstreamer/tests/examples/queue/queue.c. It is licensed under
# the terms of the GNU Library General Public License, version
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

# This example uses the queue element to create a buffer between 2 elements.
# The scheduler automatically uses 2 threads, 1 to feed and another to consume
# data from the queue buffer

# Event loop to listen to events posted on the Gst::Bus from the pipeline. Exits
# on EOS or ERROR events
def event_loop(pipe)
  running = true
  bus = pipe.bus

  while running
    message = bus.poll(Gst::MessageType::ANY, -1)

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

# create a disk reader
filesrc = Gst::ElementFactory.make("filesrc", "disk_source")
raise "'filesrc' gstreamer plugin missing" if filesrc.nil?
filesrc.location = ARGV.first

decode = Gst::ElementFactory.make("mad", "decode")
raise "'mad' gstreamer plugin missing" if decode.nil?

queue = Gst::ElementFactory.make("queue", "queue")
raise "'queue' gstreamer plugin missing" if queue.nil?

# and an audio sink
audiosink = Gst::ElementFactory.make("alsasink", "play_audio")
raise "'alsasink' gstreamer plugin missing" if audiosink.nil?

# add objects to the main pipeline
pipeline << filesrc << decode << queue << audiosink
filesrc >> decode >> queue >> audiosink

# start playing
pipeline.play

# listen for EOS
event_loop(pipeline)

pipeline.stop
