# Copyright (C) 2015-2022  Ruby-GNOME Project Team
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

class TestGtkButton < Test::Unit::TestCase
  include GtkTestUtils

  sub_test_case(".new") do
    test "no argument" do
      button = Gtk::Button.new
      assert_nil(button.label)
    end

    test ":label" do
      label = "Button"
      button = Gtk::Button.new(:label => label)
      assert_equal(label, button.label)
    end

    test ":label and :use_underline" do
      label = "Button"
      button = Gtk::Button.new(:label         => label,
                               :use_underline => true)
      assert_equal([label, true],
                   [button.label, button.use_underline?])
    end

    test ":icon_name" do
      icon_name = "document-open"
      button = Gtk::Button.new(:icon_name => icon_name)
      assert_equal(icon_name, button.icon_name)
    end
  end
end
