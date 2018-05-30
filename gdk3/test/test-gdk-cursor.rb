# Copyright (C) 2015-2018  Ruby-GNOME2 Project Team
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
    test "type" do
      cursor = Gdk::Cursor.new(:arrow)
      assert_equal(Gdk::CursorType::ARROW, cursor.cursor_type)
    end

    test "type - multiple" do
      3.times do |i|
        cursor = Gdk::Cursor.new(:arrow)
        assert_equal([i, Gdk::CursorType::ARROW],
                     [i, cursor.cursor_type])
      end
    end

    test "pixbuf" do
      pixbuf = GdkPixbuf::Pixbuf.new(:file => fixture_path("ruby-gnome2-logo.png"))
      cursor = Gdk::Cursor.new(pixbuf, 0, 0)
      assert_equal(Gdk::CursorType::CURSOR_IS_PIXMAP, cursor.cursor_type)
    end

    test "surface" do
      only_gdk_version(3, 10, 0)
      surface = Cairo::ImageSurface.new(100, 100)
      cursor = Gdk::Cursor.new(surface, 0, 0)
      assert_equal(Gdk::CursorType::CURSOR_IS_PIXMAP, cursor.cursor_type)
    end

    test "name" do
      cursor = Gdk::Cursor.new("cross")
      assert_equal(Gdk::CursorType::CURSOR_IS_PIXMAP, cursor.cursor_type)
    end

    test "name - multiple" do
      3.times do |i|
        cursor = Gdk::Cursor.new("cross")
        assert_equal([i, Gdk::CursorType::CURSOR_IS_PIXMAP],
                     [i, cursor.cursor_type])
      end
    end
  end
end
