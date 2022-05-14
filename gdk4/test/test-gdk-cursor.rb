# Copyright (C) 2015-2012  Ruby-GNOME Project Team
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

class TestGdkCursor < Test::Unit::TestCase
  include GdkTestUtils

  sub_test_case ".new" do
    test "texture" do
      pixbuf = GdkPixbuf::Pixbuf.new(:file => fixture_path("ruby-gnome2-logo.png"))
      texture = Gdk::Texture.new(pixbuf)
      cursor = Gdk::Cursor.new(texture, 0, 0)
      assert_equal(texture, cursor.texture)
    end

    test "name" do
      cursor = Gdk::Cursor.new("cross")
      assert_equal("cross", cursor.name)
    end

    test "name - multiple" do
      3.times do |i|
        cursor = Gdk::Cursor.new("cross")
        assert_equal([i, "cross"],
                     [i, cursor.name])
      end
    end
  end
end
