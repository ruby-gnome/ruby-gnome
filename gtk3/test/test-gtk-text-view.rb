# Copyright (C) 2016-2017  Ruby-GNOME2 Project Team
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

class TestGtkTextView < Test::Unit::TestCase
  include GtkTestUtils

  sub_test_case "instance methods" do
    def setup
      only_gtk_version(3, 14)
      @text_buffer = Gtk::TextBuffer.new
      @text_buffer.text = "Hello"
      @text_view = Gtk::TextView.new(@text_buffer)
    end

    sub_test_case "#get_iter_at" do
      test ":location" do
        iter = @text_view.get_iter_at(:location => [0, 0])
        assert_equal("H", iter.char)
      end

      test ":position" do
        iter, trailing = @text_view.get_iter_at(:position => [0, 0])
        assert_equal(["H", 0],
                     [iter.char, trailing])
      end
    end

    sub_test_case "#get_iter_at_location" do
      test "found" do
        iter = @text_view.get_iter_at_location(0, 0)
        assert_equal("H", iter.char)
      end

      test "not found" do
        only_gtk_version(3, 20)
        assert_nil(@text_view.get_iter_at_location(100, 0))
      end
    end

    sub_test_case "#get_iter_at_position" do
      test "found" do
        iter, trailing = @text_view.get_iter_at_position(0, 0)
        assert_equal(["H", 0],
                     [iter.char, trailing])
      end

      test "not found" do
        only_gtk_version(3, 20)
        assert_nil(@text_view.get_iter_at_position(100, 0))
      end
    end
  end
end
