
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

require 'test/unit'

module Test 
module Unit 
    class TestCase
        def assert_bool(obj)
            ok = (obj.is_a?(TrueClass) or obj.is_a?(FalseClass))
            _wrap_assertion { assert ok, "not a boolean" }
        end
        def assert_instance_of(klass, obj)
            _wrap_assertion { flunk("#{obj} is a #{obj.class} (expected #{klass})") } unless obj.is_a?(klass)
        end
        def assert_raises(klass)
            _wrap_assertion do
                begin
                    yield
                    flunk "code did not raise any exception"
                rescue => e
                    flunk "code raised exception #{e} (expected #{klass})" unless e.class == klass
                end
            end
        end
    end
end
end        

require 'gst'

Gst.init or raise "Could not initialize GStreamer!"
           
module Gst
class Object 
    def test(tc)
        # Test name
        tc.assert_instance_of(String, n = name)
        unless is_a?(Gst::PadTemplate)
            tc.assert set_name("foo") == self
            tc.assert name == "foo"
            tc.assert (name = "bar") == "bar"
            tc.assert set_name(n) == self 
        end
        # Test flags
        flags = {
            destroyed? => Gst::Object::FLAG_DESTROYED,
            floating?  => Gst::Object::FLAG_FLOATING
        }
        test_flags(tc, flags)
    end
    def test_flags(tc, flags)
        tc.assert_instance_of(Fixnum, f = self.flags)
        flags.each do |ok, flag|
            tc.assert_instance_of(Fixnum,  flag)
            tc.assert_bool ok
            tc.assert flag?(flag) == ok
            if ok
                tc.assert unset_flag(flag) == self
                tc.assert flag?(flag) == false
                tc.assert set_flag(flag) == self
            else
                tc.assert set_flag(flag) == self
                tc.assert flag?(flag) == true 
                tc.assert unset_flag(flag) == self 
            end    
        end
    end
end
class Clock
    def test(tc)
        # Test active
        tc.assert_bool active?
        # Test speed (should be always float)
        tc.assert_instance_of(Float, speed)
        # Test time (should be either Fixnum or Bignum)
        tc.assert_instance_of(Integer, time)
        # Test resolution (should be either Fixnum or Bignum)
        tc.assert_instance_of(Integer, resolution)
        # Test flags
        flags = {
            can_do_single_sync?    => Gst::Clock::FLAG_CAN_DO_SINGLE_SYNC,
            can_do_single_async?   => Gst::Clock::FLAG_CAN_DO_SINGLE_ASYNC,
            can_do_periodic_sync?  => Gst::Clock::FLAG_CAN_DO_PERIODIC_SYNC,
            can_do_periodic_async? => Gst::Clock::FLAG_CAN_DO_PERIODIC_ASYNC,
            can_set_resolution?    => Gst::Clock::FLAG_CAN_SET_RESOLUTION,
            can_set_speed?         => Gst::Clock::FLAG_CAN_SET_SPEED
        }
        test_flags(tc, flags)
    end  
end  
class Element
    def test(tc)
        # Test the element as a Gst::Object
        super(tc)
        # Test clock
        tc.assert_bool requires_clock?
        tc.assert_bool b = provides_clock?
        clock.test(tc) if (clock and b)
        # Test pads
        i = 0
        each_pad { |pad| pad.test(tc); i += 1 }
        tc.assert_instance_of(Array, a = pads)
        tc.assert_equal(a.size, i)
        a.each do |pad| 
            pad.test(tc)
            tc.assert_instance_of(Gst::Pad, pad2 = get_pad(pad.name))
            pad2.test(tc)
            tc.assert_equal(pad, pad2)
        end
        # Test flags
        flags = {
            complex?      => Gst::Element::FLAG_COMPLEX,
            decoupled?    => Gst::Element::FLAG_DECOUPLED,
            thread_suggested?  => Gst::Element::FLAG_THEAD_SUGGESTED,
            has_infinite_loop? => Gst::Element::FLAG_INFINITE_LOOP,
            has_new_loopfunc?  => Gst::Element::FLAG_NEW_LOOPFUNC, 
            event_aware?  => Gst::Element::FLAG_EVENT_AWARE,
            use_threadsafe_properties? => Gst::Element::FLAG_USE_THREADSAFE_PROPERTIES
        }
        test_flags(tc, flags)
        # Test indexable
        tc.assert_bool indexable?
    end
end        
class ElementFactory
    def test(tc) 
        # Test as a PluginFeature
        super(tc)
        # Test the string representation
        tc.assert_instance_of(String, to_s)
        # Test ranks
        ranks = {
            Gst::ElementFactory::RANK_MARGINAL  => rank_marginal?, 
            Gst::ElementFactory::RANK_NONE      => rank_none?,
            Gst::ElementFactory::RANK_PRIMARY   => rank_primary?,
            Gst::ElementFactory::RANK_SECONDARY => rank_secondary?
        }
        tc.assert_instance_of(Fixnum, r = rank) 
        ranks.each do |flag, ok|
            tc.assert_bool ok
            tc.assert_instance_of(Fixnum, flag)
            if ok
                tc.assert_equal(r, flag)
            else
                tc.assert_not_equal(r, flag)
            end
        end
        tc.assert_equal(ranks.values.collect { |v| v if v }.nitems, 1)
        # Test details
        tc.assert_instance_of(Hash, h = details) 
        keys = [ 
            "longname",
            "klass",
            "license",
            "description",
            "version",
            "author",
            "copyright"
        ]
        keys.each do |key|
            tc.assert_instance_of(String, h[key]) if h[key] 
        end
        tc.assert h.keys.size == keys.size
        # Test pad templates
        i = 0
        each_pad_template { |pad| pad.test(tc); i += 1 }
        tc.assert_instance_of(Array, a = pad_templates)
        tc.assert_equal(a.size, i)
        a.each { |pad| pad.test(tc) }
        # Test element creation
        tc.assert_instance_of(Gst::Element, e = create("an_element_#{name}"))
        e.test(tc)
        a = Array.new
        # Test unique auto-generated element names
        10.times { a.push(create) } 
        a.each { |e| tc.assert_instance_of(Gst::Element, e) }
        tc.assert_nil a.collect { |e| e.name }.uniq! 
    end
end
class Bin
    def test(tc) 
        # Test the bin as a Gst::Element
        super(tc)
        # Test clock
        clock.test(tc) if clock
        # Test flags
        flags = {
            manager?     => Gst::Bin::FLAG_MANAGER,
            schedulable? => Gst::Bin::FLAG_SCHEDULABLE,
            prefers_cothreads? => Gst::Bin::FLAG_PREFER_COTHREADS,
            has_fixed_clock?   => Gst::Bin::FLAG_FIXED_CLOCK
        }
        test_flags(tc, flags)
    end
end
class Plugin
    def test(tc)
        # Test loaded
        tc.assert_bool loaded?
        # Test name
        tc.assert_instance_of(String, n = name) 
        # Test longname (may be nil)
        if ln = longname
            tc.assert_instance_of(String, ln)
        end
        # Test filename (may be nil)
        if fn = filename
            tc.assert_instance_of(String, fn)
        end
        # Test features 
        i = 0
        each_feature { |f| f.test(tc); i += 1 }
        tc.assert_instance_of(Array, a = features) 
        tc.assert_equal(a.length, i)
        #a.each { |f| f.test(tc) }
    end
end
class PluginFeature
    def test(tc)
        # Test name
        tc.assert_instance_of(String, name) 
    end
end
class Format
    def test(tc)
        tc.assert_instance_of(Fixnum, id)
        tc.assert_instance_of(String, nick)
        tc.assert_instance_of(String, description)
        tc.assert Gst::Format.find(nick) == self 
    end
end
class QueryType
    def test(tc)
        tc.assert_instance_of(Fixnum, id)
        tc.assert_instance_of(String, nick)
        tc.assert_instance_of(String, description)
        tc.assert Gst::QueryType.find(nick) == self 
    end
end
class Pad
    def test(tc)
        # Test the pad as a Gst::Object
        super(tc)
        # Test name
        tc.assert_instance_of(String, name)
        # Test direction
        valid_directions = [
            Gst::Pad::DIRECTION_SRC,
            Gst::Pad::DIRECTION_SINK
        ]
        tc.assert_instance_of(Fixnum, dir  = direction)
        tc.assert valid_directions.include?(dir) 
        # Test pad template
        if pt = pad_template
            tc.assert_instance_of(Gst::PadTemplate, pt)
            pt.test(tc)
        end
        # Test formats
        i = 0
        each_format { |f| f.test(tc); i += 1 } 
        tc.assert_instance_of(Array, a  = formats)
        tc.assert a.length == i
        tc.assert_bool b = provides_formats?
        tc.assert a.length == 0 unless b
        a.each { |f| f.test(tc) }
        # Test query types
        i = 0
        each_query_type { |f| f.test(tc); i += 1 } 
        tc.assert_instance_of(Array, a  = query_types)
        tc.assert_equal(a.length, i)
        tc.assert_bool b = provides_query_types?
        tc.assert_equal(a.length, 0) unless b
        a.each { |f| f.test(tc) }
        # XXX Test event masks 
        # Test flags
        flags = {
            disabled?    => Gst::Pad::FLAG_DISABLED,
            negotiating? => Gst::Pad::FLAG_NEGOTIATING
        }
        test_flags(tc, flags)
    end
end
class PadTemplate
    def test(tc)
        # Test the padtemplate as a Gst::Object
        super(tc)
        # Test direction
        valid_directions = [
            Gst::Pad::DIRECTION_SRC,
            Gst::Pad::DIRECTION_SINK
        ]
        tc.assert_instance_of(Fixnum, dir = direction)
        tc.assert valid_directions.include?(dir) 
        # Test presence
        valid_presences = [
            Gst::Pad::PRESENCE_ALWAYS,
            Gst::Pad::PRESENCE_SOMETIMES,
            Gst::Pad::PRESENCE_REQUEST
        ]
        tc.assert_instance_of(Fixnum, pres  = presence)
        tc.assert valid_presences.include?(pres) 
        # Test caps
        tc.assert_bool b = has_caps?
        i = 0
        each_caps { |cap| cap.test(tc); i += 1 }
        tc.assert_instance_of(Array, a  = caps)
        tc.assert_equal(a.size, i)
        a.each { |cap| cap.test(tc) }
        tc.assert i > 0 if b
    end
end
class Caps
    def test(tc)
        # Test name 
        tc.assert_instance_of(String, name)
        # Test type id
        tc.assert_instance_of(Fixnum, type_id)
        # Test fixed
        tc.assert_bool fixed?
        # Test properties
        each_property do |name, value, is_fixed|
            tc.assert_instance_of(String, name)
            tc.assert_bool is_fixed
            tc.assert has_property?(name)    
            tc.assert_equal(has_fixed_property?(name), is_fixed)
        end
        # These tests are currently disabled to avoid critical messages.
        # But they work ;-)
        #tc.assert !has_property?("does_not_exist")
        #tc.assert !has_fixed_property?("does_not_exist_too")
    end
end
class Type
    def test(tc)
        # Test id
        tc.assert_instance_of(Fixnum, id)
        # Test mime (may be nil)
        tc.assert_instance_of(String, mime) if mime
        # Test exts (may be nil)
        tc.assert_instance_of(String, exts) if exts
        # Test Gst::Type find methods            
        a = [ Gst::Type.find_by_id(id), Gst::Type.find_by_mime(mime) ] 
        a.each do |t| 
            tc.assert_instance_of(Gst::Type, t)
            tc.assert_equal(self, t)
        end
    end
end
class TypeFactory
    def test(tc)
        # Test as a PluginFeature
        super(tc)
        # Test the string representation
        tc.assert_instance_of(String, to_s)
        # Test mime
        tc.assert_instance_of(String, mime) 
        # Test exts (may be nil)
        tc.assert_instance_of(String, exts) if exts 
    end
end   
class SchedulerFactory
    def test(tc)
        # Test as a PluginFeature
        super(tc)
        # Test the string representation
        tc.assert_instance_of(String, to_s)
    end
end   
class AutoplugFactory
    def test(tc)
        # Test as a PluginFeature
        super(tc)
        # Test the string representation
        tc.assert_instance_of(String, to_s)
    end
end   
class IndexFactory
    def test(tc)
        # Test as a PluginFeature
        super(tc)
        # Test the string representation
        tc.assert_instance_of(String, to_s)
    end
end   
class Registry
    def test(tc)
        tc.assert_instance_of(String, name)
        tc.assert_instance_of(String, details) if details
        tc.assert_bool loaded?
        i = 0
        each_path do |p|
            tc.assert_instance_of(String, p)
            i += 1
        end
        tc.assert_instance_of(Array, a = paths)
        tc.assert_equal(a.size, i)
        a.each { |p| tc.assert_instance_of(String, p) }
    end
end
end

