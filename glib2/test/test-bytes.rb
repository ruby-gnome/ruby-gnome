# Copyright (C) 2017  Ruby-GNOME2 Project Team
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

  setup do
    only_glib_version(2, 32, 0)
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
