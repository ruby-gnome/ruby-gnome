# Copyright (C) 2015  Ruby-GNOME2 Project Team
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

class TestGtkToggleButton < Test::Unit::TestCase
  include GtkTestUtils

  sub_test_case ".new" do
    test "no argument" do
      button = Gtk::ToggleButton.new
      assert_nil(button.label)
    end

    test "label" do
      button = Gtk::ToggleButton.new(:label => "label")
      assert_equal("label", button.label)
    end

    test "label + mnemonic" do
      button = Gtk::ToggleButton.new(:label => "_label", :use_underline => true)
      assert_equal("_label", button.label)
      assert do
        button.use_underline?
      end
    end
  end
end
