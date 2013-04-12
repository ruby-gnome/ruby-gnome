
# Copyright (C) 2003 Laurent Sansonetti <lrz@gnome.org>
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

class TC_xml < Test::Unit::TestCase
    TEST_FILE = "/tmp/rbgstxmltest"
    def test_xml
        src = Gst::ElementFactory.make("fakesrc", "the_source")
        sink = Gst::ElementFactory.make("fakesink", "the_sink")
        assert_not_nil src
        assert_not_nil sink
        src >> sink
        pipe = Gst::Pipeline.new("the_pipeline")
        pipe.add(src, sink)

        assert Gst::XML.write_file(pipe, TEST_FILE), 
               "could not write pipeline in #{TEST_FILE}"

        assert_instance_of(Gst::XML, parser = Gst::XML.new)
        assert parser.parse_file(TEST_FILE)
        pipe = parser.get_element("the_pipeline")
        assert_instance_of(Gst::Pipeline, pipe)
        assert pipe.size == 2
        assert_instance_of(Gst::Element, pipe.get_by_name("the_source")) 
        assert_instance_of(Gst::Element, pipe.get_by_name("the_sink"))

        i = 0 
        check_proc = Proc.new do |x|
            assert_instance_of(Gst::Element, x)
            i += 1
            if x.is_a?(Gst::Bin)
                i += x.size
            end
        end
        parser.each_topelement { |x| check_proc.call(x) }
        assert_equal(3, i)
        i = 0 
        parser.topelements.each { |x| check_proc.call(x) }
        assert_equal(3, i)

        pipe.play
        100.times { assert pipe.iterate }
        pipe.stop
    end
end

