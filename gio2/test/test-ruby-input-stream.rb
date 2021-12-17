# Copyright (C) 2021  Ruby-GNOME Project Team
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

class TestRubyInputStream < Test::Unit::TestCase
  def setup
    @base_stream = StringIO.new("Hello!")
    Gio::RubyInputStream.open(@base_stream) do |stream|
      @stream = stream
      yield
    end
  end

  sub_test_case("#read") do
    def test_with_size
      assert_equal("H", @stream.read(1))
      assert_equal("ell", @stream.read(3))
    end

    def test_without_size
      assert_equal("Hello!", @stream.read)
    end
  end

  def test_skip
    assert_equal(1, @stream.skip(1))
    assert_equal("ell", @stream.read(3))
  end

  def test_tell
    assert_equal(0, @stream.tell)
    assert_equal(1, @stream.skip(1))
    assert_equal(1, @stream.tell)
  end

  def test_can_seek?
    assert do
      @stream.can_seek?
    end
  end

  def test_seek
    assert do
      @stream.seek(-1, GLib::IOChannel::SEEK_END)
    end
    assert_equal(@base_stream.string.bytesize - 1,
                 @stream.tell)
  end

  def test_can_truncate?
    assert do
      @stream.can_truncate?
    end
  end

  def test_truncate
    assert do
      @stream.truncate(2)
    end
    assert_equal("He", @stream.read)
  end
end
