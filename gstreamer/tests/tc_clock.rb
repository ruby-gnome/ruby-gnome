
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
    def debug(c)
        puts "Clock info: speed #{c.speed}, active #{c.active?}, time #{c.time}" if $VERBOSE
    end
    def test_system_clock
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
            a, b = (x.succ - 0.1) * Gst::SECOND, (x.succ + 0.1) * Gst::SECOND 
            assert (clock.time >= a && clock.time <= b)
        end
    end
    def test_desynch2
        clock = Gst::SystemClock.obtain
        assert_instance_of(Gst::Clock, clock)
        clock.reset!
        assert !clock.active?
        100.times { assert_equal(0, clock.time) }
        clock.active = true
        assert clock.active?
        100.times do 
            assert clock.time > 0
            assert_equal(clock, Gst::SystemClock.obtain)
        end
        clock.active = false
        assert !clock.active?
        now = clock.time
        100.times { assert_equal(now, clock.time) }
        clock.active = true
        clock.reset!
        assert !clock.active?
        100.times { assert_equal(0, clock.time) }
    end
    def test_timer_sync
        ret_codes = [
            Gst::Clock::STOPPED,
            Gst::Clock::TIMEOUT,
            Gst::Clock::EARLY,
            Gst::Clock::ERROR,
            Gst::Clock::UNSUPPORTED
        ]   
        clock = Gst::SystemClock.obtain
        assert_instance_of(Gst::Clock, clock)

        # one shot
        
        clock.reset!
        clock.active = true
        id = Gst::ClockEntry.new(clock, 2 * Gst::SECOND)
        assert id.single_shot?
        assert !id.periodic?
        assert_equal(2 * Gst::SECOND, id.time)
        #assert_equal(0, id.interval)  # does not work yet, see below
        assert_equal(clock, id.clock)
        [ 2, 0, 0, 0 ].each do |desired_interval|
            now = Time.now.to_i
            assert ret_codes.include?(c = id.wait)
            elapsed = Time.now.to_i - now
            assert_equal(desired_interval, elapsed)
        end

        # periodic (does not work yet, because of 'interval' bug.
        # http://bugzilla.gnome.org/show_bug.cgi?id=121015

#        clock.reset!
#        clock.active = true
#        id = Gst::ClockEntry.new(clock, 1 * Gst::SECOND, 2 * Gst::SECOND)
#        assert id.periodic?
#        assert !id.single_shot?
#        assert_equal(2 * Gst::SECOND, id.time)
#        assert_equal(1 * Gst::SECOND, id.interval)
#        assert_equal(clock, id.clock)
#        [ 2, 2 ].each do |desired_interval|
#            now = Time.now.to_i
#            assert ret_codes.include?(c = id.wait)
#            elapsed = Time.now.to_i - now
#            assert_equal(desired_interval, elapsed)
#        end
    end
    def test_timer_async
        # XXX
        # can't be tested on Gst::SystemClock since it does not support
        # asynchronous notifications yet.
    end
end

