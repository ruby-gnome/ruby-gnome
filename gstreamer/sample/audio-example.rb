#!/usr/bin/env ruby
# -*- coding: utf-8 -*-
#
# audio-example.rb
#
# Builds a pipeline with [ audiotestsource ! autoaudiosink ] and sweeps
# frequency and volume.
#
# Needs gst-plugin-base + gst-plugins-good installed.
#
# This sample code is a port of
# gstreamer/tests/examples/controller/audio-example.c. It is licensed
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

Gst.init

bin = Gst::Pipeline.new("pipeline")
clock = bin.clock
src = Gst::ElementFactory.make("audiotestsrc", nil)
raise "need audiotestsrc from gst-plugins-base" if src.nil?
sink = Gst::ElementFactory.make("autoaudiosink", nil)
raise "need autoaudiosink from gst-plugins-good" if sink.nil?

bin << src << sink
src >> sink

# setup control sources
cs1 = Gst::InterpolationControlSource.new
cs2 = Gst::InterpolationControlSource.new

src.add_control_binding(Gst::DirectControlBinding.new(src, "volume", cs1))
src.add_control_binding(Gst::DirectControlBinding.new(src, "freq", cs2))

# set interpolation mode
cs1.mode = Gst::InterpolationMode::LINEAR
cs2.mode = Gst::InterpolationMode::LINEAR

# set control values
cs1.set(0 * Gst::SECOND, 0.0)
cs1.set(5 * Gst::SECOND, 1.0)

cs2.set(0 * Gst::SECOND,  220.0 / 20000.0)
cs2.set(3 * Gst::SECOND, 3520.0 / 20000.0)
cs2.set(6 * Gst::SECOND,  440.0 / 20000.0)

# run for 7 seconds
clock_id = clock.new_single_shot_id(clock.time + (7 * Gst::SECOND))
bin.play
wait_ret, jitter = Gst::Clock.id_wait(clock_id)
$stderr.puts "Clock::id_wait returned: #{wait_ret}" if wait_ret != Gst::ClockReturn::OK
bin.stop
