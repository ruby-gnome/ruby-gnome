#!/usr/bin/env ruby
#
# This sample code is a port of clutter-gst/examples/video-sink.c.
# It is licensed under the terms of the GNU Lesser General Public
# License, version 2.1 or (at your option) later.
#
# The original header:
#   video-sink.c - A small example around the videotestsrc ! warptv !
#                  videoconvert ! cluttersink pipeline.
#
#   Copyright (C) 2007,2008 OpenedHand
#
# Copyright (C) 2013-2014  Ruby-GNOME2 Project Team
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

require "clutter-gst"

ClutterGst.load_version = "2.0"

stage = Clutter::Stage.new
stage.signal_connect("destroy") do
  Clutter.main_quit
end

# Make a timeline
timeline = Clutter::Timeline.new(1000)
timeline.loop = true

# We need to set certain props on the target texture currently for
# efficient/correct playback onto the texture (which sucks a bit)
texture = Clutter::Texture.new
# NOTE: Because warning is given, I comment out.
#texture.set_property("disable-slicing", true)

texture.signal_connect("size-change") do |widget, width, height|
  stage = widget.stage
  next if stage.nil?  # go out of the block if stage is nil
  stage_width, stage_height = stage.size
  new_height = (height * stage_width) / width
  if new_height <= stage_height
    new_width = stage_width
    new_x = 0
    new_y = (stage_height - new_height) / 2
  else
    new_width = (width * stage_height) / height
    new_height = stage_height
    new_x = (stage_width - new_width) / 2
    new_y = 0
  end
  widget.set_position(new_x, new_y)
  widget.set_size(new_width, new_height)
end

# Set up pipeline
pipeline = Gst::Pipeline.new("pipeline")

src = Gst::ElementFactory.make("videotestsrc")
warp = Gst::ElementFactory.make("warptv")
colorspace = Gst::ElementFactory.make("videoconvert")
sink = ClutterGst::VideoSink.new(texture)

# We can try other patterns
# src.pattern = 10

pipeline << src << warp << colorspace << sink
src >> warp >> colorspace >> sink
pipeline.play

# start the timeline
timeline.start

stage.add_child(texture)
# texture.opacity = 0x11
stage.show

Clutter.main
