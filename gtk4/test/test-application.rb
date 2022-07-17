# Copyright (C) 2018  Ruby-GNOME2 Project Team
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

class TestGtkApplication < Test::Unit::TestCase
  include GtkTestUtils

  sub_test_case(".new") do
    test "none" do
      app = Gtk::Application.new
      assert_equal([
                     nil,
                     Gio::ApplicationFlags::FLAGS_NONE,
                   ],
                   [
                     app.application_id,
                     app.flags,
                   ])
    end

    test "id" do
      app = Gtk::Application.new("org.example.test")
      assert_equal([
                     "org.example.test",
                     Gio::ApplicationFlags::FLAGS_NONE,
                   ],
                   [
                     app.application_id,
                     app.flags,
                   ])
    end

    test "flags" do
      app = Gtk::Application.new(:handles_command_line)
      assert_equal([
                     nil,
                     Gio::ApplicationFlags::HANDLES_COMMAND_LINE,
                   ],
                   [
                     app.application_id,
                     app.flags,
                   ])
    end

    test "id, flags" do
      app = Gtk::Application.new("org.example.test", :handles_command_line)
      assert_equal([
                     "org.example.test",
                     Gio::ApplicationFlags::HANDLES_COMMAND_LINE,
                   ],
                   [
                     app.application_id,
                     app.flags,
                   ])
    end
  end
end
