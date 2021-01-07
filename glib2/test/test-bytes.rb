# Copyright (C) 2017-2021  Ruby-GNOME Project Team
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

class TestGLibBytes < Test::Unit::TestCase
  include GLibTestUtils

  sub_test_case ".try_convert" do
    def test_nil
      assert_nil(GLib::Bytes.try_convert(nil))
    end

    def test_bytes
      bytes = GLib::Bytes.new("Hello")
      assert_equal(bytes,
                   GLib::Bytes.try_convert(bytes))
    end

    def test_string
      assert_equal("Hello",
                   GLib::Bytes.try_convert("Hello").to_s)
    end

    def test_unconvertable
      assert_nil(GLib::Bytes.try_convert([]))
    end
  end

  sub_test_case ".new" do
    def create_bytes(options={})
      data = "Hello"
      data.freeze if options[:freeze]
      [data.object_id, GLib::Bytes.new(data)]
    end

    test "frozen" do
      id, bytes = create_bytes(:freeze => true)
      GC.start
      assert_equal(bytes.to_s,
                   ObjectSpace._id2ref(id))
    end
  end

  test "#to_s" do
    data = "Hello"
    bytes = GLib::Bytes.new(data)
    string = bytes.to_s
    assert_equal([data, Encoding::ASCII_8BIT],
                 [string, string.encoding])
  end

  test "#to_str" do
    bytes = GLib::Bytes.new("Hello")
    assert_equal(bytes.to_s, bytes.to_str)
  end
end
