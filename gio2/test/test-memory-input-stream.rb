# Copyright (C) 2014-2026  Ruby-GNOME Project Team
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

class TestMemoryInputStream < Test::Unit::TestCase
  def test_memory_input_stream
    assert_nothing_raised do
      Gio::MemoryInputStream.new
    end
  end

  def test_new_string
    stream = Gio::MemoryInputStream.new("data")
    assert_equal("data", stream.read_bytes(4).to_s)
  end

  def test_new_bytes
    stream = Gio::MemoryInputStream.new(GLib::Bytes.new("data"))
    assert_equal("data", stream.read_bytes(4).to_s)
  end
end
