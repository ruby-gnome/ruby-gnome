
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

class TC_modfunc < Test::Unit::TestCase
    def test_version
        assert_instance_of(Array, ver = Gst.version)
        assert_equal(ver.size, 3)
        ver.each { |i| assert_instance_of(Fixnum, i) }
    end
    def test_threads
        b = Gst.has_threads?
        assert_bool b
        assert_equal(Gst.use_threads(!b), !b)
        assert_equal(!b, Gst.has_threads?)
        assert_equal(Gst.use_threads(b), b)        
        assert_equal(b, Gst.has_threads?)
    end
end

