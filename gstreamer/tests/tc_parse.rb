
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

class TC_parse < Test::Unit::TestCase
    def test_launch
        assert_instance_of(Gst::Bin, Gst::Parse.launch("fakesrc ! fakesink"))
        assert_raises(RuntimeError) { Gst::Parse.launch("plok") }
    end
end

