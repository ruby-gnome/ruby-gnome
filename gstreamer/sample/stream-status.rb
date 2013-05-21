#!/usr/bin/env ruby
# -*- coding: utf-8 -*-
#
# This sample code is a port of
# gstreamer/tests/examples/streams/stream-status.c. It is licensed
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

def event_loop(bus, pipe)

  loop do
    message = bus.poll(Gst::MessageType::ANY, Gst::CLOCK_TIME_NONE)
    raise "message nil" if message.nil?

    case message.type
    when Gst::MessageType::EOS
      puts "received EOS"
      return
    when Gst::MessageType::WARNING
      warning, debug = message.parse_warning
      puts "#{message} #{warning.message}"
    when Gst::MessageType::ERROR
      error, debug = message.parse_error
      puts "#{message} #{error.message}"
      return
    end
  end

end

# create a new bin to hold the elements
bin = Gst::Pipeline.new("pipeline")
raise "'pipeline' gstreamer plugin missing" if bin.nil?

# create a source
fakesrc = Gst::ElementFactory.make("fakesrc", "fakesrc")
raise "'fakesrc' gstreamer plugin missing" if fakesrc.nil?

# and a sink
fakesink = Gst::ElementFactory.make("fakesink", "fakesink")
raise "'fakesink' gstreamer plugin missing" if fakesink.nil?

# add objects to the main pipeline
bin << fakesrc << fakesink

# link the elements
fakesrc >> fakesink

# get the bus, we need to install a sync handler
bin.bus.sync_handler do |bus, message|

  case message.type
  when Gst::MessageType::STREAM_STATUS
    type, owner = message.parse_stream_status

    val = message.stream_status_object

    puts "type:   #{type}"
    puts "source: #{message.src.path_string}"
    puts "owner:  #{owner.path_string}"
    puts "object: type #{val.type.name}, value #{val.value}"

    # see if we know how to deal with this object
    if val.type == Gst::Task
      task = val.value
    end

    case type
    when Gst::StreamStatusType::CREATE
      puts "created task #{task}"
    when Gst::StreamStatusType::ENTER
      # puts "raising task priority"
      # setpriority(PRIO_PROCESS, 0, -10)
    when Gst::StreamStatusType::LEAVE
    end
  end

  Gst::BusSyncReply::PASS
end

# start playing
bin.play

# run event loop listening for bus messages until EOS or ERROR
event_loop(bus, bin)

# stop the bin
bin.stop
