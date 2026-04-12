# Copyright (C) 2026  Ruby-GNOME Project Team
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

class TestBuffer < Test::Unit::TestCase
  def setup
    unless Gst::ElementFactory.find("videotestsrc")
      omit("gst-plugins-good is needed")
    end
    @pipeline = Gst.parse_launch("videotestsrc ! appsink name=sink")
    @sink = @pipeline.get_by_name("sink")
  end

  def test_map
    @pipeline.play
    begin
      sample = @sink.try_pull_sample(Gst::SECOND)
      buffer = sample.buffer
      map_info = buffer.map(:read)

      assert do
        map_info.data.size >= 115200
      end
    ensure
      @pipeline.stop
    end
  end

  def test_map_with_block
    @pipeline.play
    begin
      sample = @sink.try_pull_sample(Gst::SECOND)
      buffer = sample.buffer
      buffer.map(:read) do |map_info|
        assert do
          map_info.data.size >= 115200
        end
      end
    ensure
      @pipeline.stop
    end
  end

  def test_map_fail
    @pipeline.play
    begin
      sample = @sink.try_pull_sample(Gst::SECOND)
      buffer = sample.buffer

      assert_raise(Gst::CoreError::Failed) do
        buffer.map(:write)
      end
    ensure
      @pipeline.stop
    end
  end

  def test_map_range_full
    @pipeline.play
    begin
      sample = @sink.try_pull_sample(Gst::SECOND)
      buffer = sample.buffer
      map_info = buffer.map_range(0, -1, :read)

      assert do
        map_info.data.size >= 115200
      end
    ensure
      @pipeline.stop
    end
  end

  def test_map_range_with_block
    @pipeline.play
    begin
      sample = @sink.try_pull_sample(Gst::SECOND)
      buffer = sample.buffer
      buffer.map_range(0, -1, :read) do |map_info|
        assert do
          map_info.data.size >= 115200
        end
      end
    ensure
      @pipeline.stop
    end
  end

  def test_map_range_full_fail
    @pipeline.play
    begin
      sample = @sink.try_pull_sample(Gst::SECOND)
      buffer = sample.buffer

      assert_raise(Gst::CoreError::Failed) do
        buffer.map_range(0, -1, :write)
      end
    ensure
      @pipeline.stop
    end
  end

  def test_map_range_partial
    allocator = Gst::Allocator.find
    memory1 = allocator.alloc(100)
    memory2 = allocator.alloc(200)
    memory3 = allocator.alloc(300)
    buffer = Gst::Buffer.new
    buffer.append_memory(memory1)
    buffer.append_memory(memory2)
    buffer.append_memory(memory3)
    map_info = buffer.map_range(1, 2, :read)

    assert_equal(500, map_info.data.size)
  end
end
