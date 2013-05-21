#!/usr/bin/env ruby
# -*- coding: utf-8 -*-
#
# This sample code is a port of
# gstreamer/tests/examples/stepping/framestep1.c. It is licensed
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

  while running
    message = bus.timed_pop_filtered(Gst::CLOCK_TIME_NONE, Gst::MessageType::ANY)
    raise "message nil" if message.nil?

    case message.type
    when Gst::MessageType::EOS
      puts "got EOS"
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
    when Gst::MessageType::STEP_DONE
      format, amount, rate, flush, intermediate, duration, eos =
        message.parse_step_done
      if format == Gst::Format::DEFAULT
        puts "step done: #{gst_time(duration)} skipped in #{gst_time(amount)} frames"
      else
        puts "step done: #{gst_time(duration)} skipped"
      end
      running = false
    end
  end
end

def gst_time(time)
  Time.at(time / 1000000000.0).utc.strftime("%H:%M:%S.%N")
end

# create a new bin to hold the elements
bin = Gst::Pipeline.new("pipeline")
raise "'pipeline' gstreamer plugin missing" if bin.nil?

# create a fake source
videotestsrc = Gst::ElementFactory.make("videotestsrc", "videotestsrc")
raise "'videotestsrc' gstreamer plugin missing" if videotestsrc.nil?
videotestsrc.num_buffers = 10

# and a fake sink
appsink = Gst::ElementFactory.make("appsink", "appsink")
raise "'appsink' gstreamer plugin missing" if appsink.nil?
appsink.emit_signals = true
appsink.sync = true
appsink.signal_connect("new-preroll") do |appsink|
  # signalled when a new preroll buffer is available
  sample = appsink.pull_preroll
  puts "have new-preroll sample #{sample}, timestamp #{gst_time(sample.buffer.pts)}"
  Gst::FlowReturn::OK
end

# add objects to the main pipeline
bin << videotestsrc << appsink

# link the elements
videotestsrc >> appsink

# go to the PAUSED state and wait for preroll
puts "prerolling first frame"
bin.pause
bin.get_state(Gst::CLOCK_TIME_NONE)

# step two frames, flush so that new preroll is queued
puts "stepping three frames"
unless bin.send_event(Gst::Event.new(Gst::Format::BUFFERS, 2, 1.0, true, false))
  raise "Failed to send STEP event!"
end

# blocks and returns when we received the step done message
event_loop(bin)

# wait for step to really complete
bin.get_state(Gst::CLOCK_TIME_NONE)

result, pos = bin.query_position(Gst::Format::TIME)
puts "stepped two frames, now at #{gst_time(pos)}"

# step 3 frames, flush so that new preroll is queued
puts "stepping 120 milliseconds"
unless bin.send_event(Gst::Event.new(Gst::Format::TIME, 120 * Gst::MSECOND, 1.0,
                                      true, false))
  raise "Failed to send STEP event!"
end

# blocks and returns when we received the step done message
event_loop(bin)

# wait for step to really complete
bin.get_state(Gst::CLOCK_TIME_NONE)

result, pos = bin.query_position(Gst::Format::TIME)
puts "stepped 120ms frames, now at #{gst_time(pos)}"

puts "playing until EOS"
bin.play

# Run event loop listening for bus messages until EOS or ERROR
event_loop(bin)
puts "finished"

# stop the bin
bin.stop
