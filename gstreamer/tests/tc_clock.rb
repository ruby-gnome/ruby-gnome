
# Copyright (C) 2003 Laurent Sansonetti <laurent@datarescue.be>
#
# This file is part of Ruby/GStreamer.
# 
# Ruby/GStreamer is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# Ruby/GStreamer is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with Ruby/GStreamer; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA

require 'tests/misc'

class TC_clock < Test::Unit::TestCase
    SEC = 1_000_000_000
    def debug(c)
        puts "Clock info: speed #{c.speed}, active #{c.active?}, time #{c.time}" if $VERBOSE
    end
    def test_clock
        assert_instance_of(Gst::Clock, c = Gst::SystemClock.obtain)
        c.test(self)
    end
    def test_desynch_clock
        src  = Gst::ElementFactory.make("fakesrc",  "source")
        id   = Gst::ElementFactory.make("identity", "filter")
        sink = Gst::ElementFactory.make("fakesink", "sink")
        
        pipeline = Gst::Pipeline.new("pipeline")
        
        src >> id >> sink
        pipeline.add(src, id, sink) 
        
        assert_instance_of(Gst::Clock, clock = pipeline.clock)
        clock.test(self)
        assert !clock.active?
        assert_equal(clock.time, 0)
        debug(clock)        
        
        pipeline.play

        debug(clock)        
        assert clock.active?

        5.times do |x|
            sleep 1 
            pipeline.iterate
            debug(clock)
            a, b = (x.succ - 0.1) * SEC, (x.succ + 0.1) * SEC
            assert (clock.time >= a && clock.time <= b)
        end

        # TODO: test '=='
    end
end

