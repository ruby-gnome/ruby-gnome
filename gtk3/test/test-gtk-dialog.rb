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

class TestGtkDialog < Test::Unit::TestCase
  include GtkTestUtils

  sub_test_case ".new" do
    test "no argument" do
      dialog = Gtk::Dialog.new
      assert_nil(dialog.title)
    end

    test "title" do
      dialog = Gtk::Dialog.new(:title => "title")
      assert_equal("title", dialog.title)
    end

    test "parent" do
      parent = Gtk::Window.new
      dialog = Gtk::Dialog.new(:parent => parent)
      assert_equal(parent, dialog.transient_for)
    end

    test "flags - modal" do
      dialog = Gtk::Dialog.new(:flags => Gtk::DialogFlags::MODAL)
      assert do
        dialog.modal?
      end
    end

    test "flags - destroy_with_parent" do
      dialog = Gtk::Dialog.new(:flags => Gtk::DialogFlags::DESTROY_WITH_PARENT)
      assert do
        dialog.destroy_with_parent?
      end
    end

    test "buttons" do
      response_id = 1
      buttons = [
        ["title", response_id],
      ]
      dialog = Gtk::Dialog.new(:buttons => buttons)
      button = dialog.get_widget_for_response(response_id)
      assert_equal("title", button.label)
    end
  end

  sub_test_case "instance methods" do
    def setup
      @dialog = Gtk::Dialog.new
    end

    test "#use_header_bar?" do
      only_gtk_version(3, 12, 0)
      assert do
        not @dialog.use_header_bar?
      end
    end
  end
end
