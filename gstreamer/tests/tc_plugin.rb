
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

class TC_plugins < Test::Unit::TestCase
    def test_registry
        i = 0
        Gst::Registry.each do |r| 
            assert_instance_of(Gst::Registry, r)
            r.test(self)
            i += 1
        end
        assert_instance_of(Array, a = Gst::Registry.registries)
        assert a.size == i
        a.each { |r| assert_instance_of(Gst::Registry, r); r.test(self) }
    end
    def test_plugins
        i = 0
        Gst::Registry.each_plugin do |p| 
            assert_instance_of(Gst::Plugin, p)
            assert_instance_of(Gst::Plugin, p2 = Gst::Registry.find_plugin(p.name))
            assert_equal(p, p2)
            p.test(self)  # note: takes time
            i += 1
        end
        assert_instance_of(Array, a  = Gst::Registry.plugins)
        assert_equal(a.size, i)
        a.each { |p| assert_instance_of(Gst::Plugin, p) }
        assert_instance_of(Gst::Plugin, p = Gst::Registry::find_plugin("gstelements"))
        assert_nil Gst::Registry.find_plugin("does not exist")
    end
end

