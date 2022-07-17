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

class TestGtkRadioButton < Test::Unit::TestCase
  include GtkTestUtils

  sub_test_case ".new" do
    test "no argument" do
      button = Gtk::RadioButton.new
      assert_nil(button.label)
    end

    test "label" do
      button = Gtk::RadioButton.new(:label => "label")
      assert_equal("label", button.label)
    end

    test "label + use_underline" do
      button = Gtk::RadioButton.new(:label => "_label", :use_underline => true)
      assert do
        button.use_underline?
      end
    end

    test "member" do
      button1 = Gtk::RadioButton.new
      button2 = Gtk::RadioButton.new(:member => button1)
      assert_equal([
                     [button2, button1],
                     [button2, button1],
                   ],
                   [
                     button1.group,
                     button2.group,
                   ])
    end

    test "member + label" do
      button1 = Gtk::RadioButton.new
      button2 = Gtk::RadioButton.new(:member => button1,
                                     :label => "label")
      assert_equal({
                     :label => "label",
                     :group => [button2, button1],
                   },
                   {
                     :label => button2.label,
                     :group => button2.group,
                   })
    end

    test "member + label + use_underline" do
      button1 = Gtk::RadioButton.new
      button2 = Gtk::RadioButton.new(:member => button1,
                                     :label => "_label",
                                     :use_underline => true)
      assert_equal({
                     :label => "_label",
                     :group => [button2, button1],
                     :use_underline => true,
                   },
                   {
                     :label => button2.label,
                     :group => button2.group,
                     :use_underline => button2.use_underline?,
                   })
    end
  end
end
