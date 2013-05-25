#!/usr/bin/env ruby
# -*- coding: utf-8 -*-
#
# text-color-example.rb
#
# Builds a pipeline with [videotestsrc ! textoverlay ! ximagesink] and
# modulates color, text and text pos.
#
# Needs gst-plugin-base installed.
#
# This sample code is a port of
# gstreamer/tests/examples/controller/text-color-example.c. It is
# licensed under the terms of the GNU Library General Public License,
# version 2 or (at your option) later.
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

bin = Gst::Pipeline.new("pipeline")
clock = bin.clock
src = Gst::ElementFactory.make("videotestsrc", nil)
raise "need videotestsrc from gst-plugins-base" if src.nil?
src.pattern = 10 # checkers-8
text = Gst::ElementFactory.make("textoverlay", nil)
raise "need textoverlay from gst-plugins-base" if text.nil?
text.text = "GStreamer rocks!"
text.font_desc = "Sans, 30"
text.halignment = 4 # position
text.valignment = 3 # position
sink = Gst::ElementFactory.make("ximagesink", nil)
raise "need ximagesink from gst-plugins-base" if sink.nil?

bin << src << text << sink
src >> text >> sink

# setup control sources
cs = Gst::LFOControlSource.new
cs.frequency = 0.11
cs.amplitude = 0.2
cs.offset = 0.5
text.add_control_binding(Gst::DirectControlBinding.new(text, "xpos", cs))

cs = Gst::LFOControlSource.new
cs.frequency = 0.04
cs.amplitude = 0.4
cs.offset = 0.5
text.add_control_binding(Gst::DirectControlBinding.new(text, "ypos", cs))

cs_r = Gst::LFOControlSource.new
cs_r.frequency = 0.19
cs_r.amplitude = 0.5
cs_r.offset = 0.5
cs_g = Gst::LFOControlSource.new
cs_g.frequency = 0.27
cs_g.amplitude = 0.5
cs_g.offset = 0.5
cs_b = Gst::LFOControlSource.new
cs_b.frequency = 0.13
cs_b.amplitude = 0.5
cs_b.offset = 0.5
text.add_control_binding(Gst::ARGBControlBinding.new(text, "color", nil,
                                                     cs_r, cs_g, cs_b))

# run for 10 seconds
clock_id = clock.new_single_shot_id(clock.time + (30 * Gst::SECOND))
bin.play
wait_ret, jitter = Gst::Clock.id_wait(clock_id)
$stderr.puts "Clock.id_wait returned: #{wait_ret}" if wait_ret != Gst::ClockReturn::OK
bin.stop
