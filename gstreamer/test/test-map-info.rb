# Copyright (C) 2016-2022  Ruby-GNOME Project Team
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

class TestMapInfo < Test::Unit::TestCase
  def setup
    @pipeline = Gst.parse_launch("videotestsrc ! appsink name=sink")
    @sink = @pipeline.get_by_name("sink")
  end

  def test_data
    @pipeline.play
    begin
      sample = @sink.try_pull_sample(Gst::SECOND)
      buffer = sample.buffer
      success, map_info = buffer.map(:read)
      assert_true(success)
      assert do
        map_info.data.size >= 115200
      end
    ensure
      @pipeline.stop
    end
  end
end
