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

class TestGtkColorChooserDialog < Test::Unit::TestCase
  include GtkTestUtils

  sub_test_case ".new" do
    test "no argument" do
      dialog = Gtk::ColorChooserDialog.new
      assert_equal("", dialog.title || "")
    end

    test "title" do
      dialog = Gtk::ColorChooserDialog.new(:title => "title")
      assert_equal("title", dialog.title)
    end

    test "parent" do
      parent = Gtk::Window.new
      dialog = Gtk::ColorChooserDialog.new(:parent => parent)
      assert_equal(parent, dialog.transient_for)
    end
  end
end
