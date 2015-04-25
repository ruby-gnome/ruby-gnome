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

class TestGtkMessageDialog < Test::Unit::TestCase
  include GtkTestUtils

  sub_test_case ".new" do
    test "no argument" do
      dialog = Gtk::MessageDialog.new
      assert_equal("", dialog.text)
    end

    test "parent" do
      parent = Gtk::Window.new
      dialog = Gtk::MessageDialog.new(:parent => parent)
      assert_equal(parent, dialog.transient_for)
    end

    test "flags - modal" do
      dialog = Gtk::MessageDialog.new(:flags => :modal)
      assert do
        dialog.modal?
      end
    end

    test "flags - destroy_with_parent" do
      dialog = Gtk::MessageDialog.new(:flags => :destroy_with_parent)
      assert do
        dialog.destroy_with_parent?
      end
    end

    test "type" do
      dialog = Gtk::MessageDialog.new(:type => :error)
      assert_equal(Gtk::MessageType::ERROR, dialog.message_type)
    end

    test "buttons" do
      dialog = Gtk::MessageDialog.new(:buttons => :cancel)
      button = dialog.get_widget_for_response(Gtk::ResponseType::CANCEL)
      assert_not_nil(button)
    end

    test "message" do
      dialog = Gtk::MessageDialog.new(:message => "Label")
      assert_equal("Label", dialog.text)
    end
  end
end
