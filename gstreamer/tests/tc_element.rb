
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

class TC_elements < Test::Unit::TestCase
    def test_element
        assert_instance_of(Gst::Element, e  = Gst::ElementFactory.make("fakesrc"))
        assert_instance_of(Gst::Element, e2 = Gst::ElementFactory.make("fakesrc", "plok"))
        e.test(self); e2.test(self)
        # Test states 
        state  = Struct.new("State", :code, :get, :set)
        states = [
    		state.new(Gst::Element::STATE_READY  , "e.ready?",   "e.ready"),
    		state.new(Gst::Element::STATE_PLAYING, "e.playing?", "e.play" ),
    		state.new(Gst::Element::STATE_PAUSED , "e.paused?",  "e.pause"),
    		state.new(Gst::Element::STATE_NULL   , "e.stopped?", "e.stop" )
        ]	
        valid_codes = [ 
            Gst::Element::STATE_FAILURE,
            Gst::Element::STATE_SUCCESS,
            Gst::Element::STATE_ASYNC
        ] 
    	states.each do |state|
            assert_instance_of(Fixnum, state.code)
            assert_instance_of(Fixnum, code = eval(state.set))
            assert valid_codes.include?(code)
            assert_equal(code, Gst::Element::STATE_SUCCESS)
            assert_instance_of(Fixnum, s = e.state)
            assert_equal(s, state.code)
            assert eval(state.get)
            states.each do | state2|
                next if state.code == state2.code
                assert !eval(state2.get)
            end
    	end
        # Test properties
        e.each_property do |key, descr, val|
            assert_instance_of(String, key)
            assert_instance_of(String, descr)
            assert_equal(e.get_property(key), val)
            #assert_equal(eval("e.#{key}"), val)
        end
        assert_raises(ArgumentError) do 
            e.get_property("does_not_exist")
        end
        assert_instance_of(String, n = e.get_property("name"))
        assert_equal(e.set_property("name", "foo"), e)
        assert_equal(e.get_property("name"), "foo")
        assert_equal(e.name = n, n)
    end
    def test_elementfactory
        assert_instance_of(Gst::ElementFactory, f = Gst::ElementFactory.find("fakesrc"))
        f.test(self)
        # Test unique auto-generated element names
        a = Array.new
        10.times { a.push(Gst::ElementFactory.make("fakesrc")) }
        a.each { |e| assert_instance_of(Gst::Element, e) }
        assert_nil a.collect { |e| e.name }.uniq! 
    end
    def test_pipeline
        assert_instance_of(Gst::Pipeline, p = Gst::Pipeline.new("my pipeline"))
        # Test the pipeline as a Gst::Bin (Gst::Pipeline does not have a #test method)
        p.test(self)
        # Test unique auto-generated pipelines names
        a = Array.new
        10.times { a.push(Gst::Pipeline.new) }
        a.each { |e| assert_instance_of(Gst::Pipeline, e) }
        assert_nil a.collect { |e| e.name }.uniq! 
    end
    def test_bin
        assert_instance_of(Gst::Bin, b = Gst::Bin.new("my bin"))
        b.test(self)
        # Test signals
        #b.signal_connect("element-added") { puts "element added!" }
        # Test container
        assert_equal(0, b.size)
        100.times { |i| b.add(Gst::ElementFactory.make("mad")) }
        assert_equal(b.size, 100)
        i = 0
        b.each_element { |e| assert_instance_of(Gst::Element, e); i += 1 }
        assert_equal(b.size, i)
        b.clear
        assert_equal(b.size, 0)
        a = Array.new
        100.times { a.push(Gst::ElementFactory.make("fakesrc")) }
        assert_equal(a.length, 100)
        b.add(*a)
        assert_equal(b.size, b.length)
        assert_equal(b.size, 100)
        assert_instance_of(Array, a = b.elements)
        assert_equal(a.length, 100)
        a.each { |e| assert_instance_of(Gst::Element, e) }
        b.remove(*a[0, 50])
        assert_equal(b.size, b.length)
        assert_equal(b.size, 50)
        b.remove_all
        assert_equal(b.size, b.length)
        assert_equal(b.size, 0)
        # Test unique auto-generated bins names
        a = Array.new
        10.times { a.push(Gst::Bin.new) }
        a.each { |e| assert_instance_of(Gst::Bin, e) }
        assert_nil a.collect { |e| e.name }.uniq! 
    end
end

