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

class TestTargetEntry < Test::Unit::TestCase
  include GtkTestUtils

  sub_test_case(".new") do
    sub_test_case("flags") do
      test("constant") do
        target_entry = Gtk::TargetEntry.new("test",
                                            Gtk::TargetFlags::SAME_APP,
                                            12_345)
        assert_equal(Gtk::TargetFlags::SAME_APP,
                     target_entry.flags)
      end

      test("symbol") do
        target_entry = Gtk::TargetEntry.new("test", :same_app, 12_345)
        assert_equal(Gtk::TargetFlags::SAME_APP,
                     target_entry.flags)
      end
    end
  end
end
