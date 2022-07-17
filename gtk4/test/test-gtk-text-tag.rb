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

class TestGtkTextTag < Test::Unit::TestCase
  include GtkTestUtils

  sub_test_case "instance methods" do
    def setup
      @tag = Gtk::TextTag.new
    end

    sub_test_case "#weight" do
      test "Symbol" do
        @tag.weight = :bold
        assert_equal(Pango::Weight::BOLD, @tag.weight)
      end

      test "String" do
        @tag.weight = "bold"
        assert_equal(Pango::Weight::BOLD, @tag.weight)
      end
    end

    sub_test_case "#scale" do
      test "Symbol" do
        @tag.scale = :x_small
        assert_equal(Pango::Scale::X_SMALL, @tag.scale)
      end

      test "String" do
        @tag.scale = "x_small"
        assert_equal(Pango::Scale::X_SMALL, @tag.scale)
      end
    end
  end
end
