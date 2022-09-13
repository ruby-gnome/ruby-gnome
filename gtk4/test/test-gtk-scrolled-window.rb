# Copyright (C) 2022  Ruby-GNOME Project Team
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

class TestGtkScrolledWindow < Test::Unit::TestCase
  include GtkTestUtils

  sub_test_case(".new") do
    test("no argument") do
      scrolled_window = Gtk::ScrolledWindow.new
      assert_equal([0, 0],
                   [
                     scrolled_window.hadjustment.value,
                     scrolled_window.vadjustment.value,
                   ])
    end

    test("hadjustment") do
      hadjustment = Gtk::Adjustment.new(50, 20, 80, 2, 20, 5)
      scrolled_window = Gtk::ScrolledWindow.new(hadjustment)
      assert_equal([50, 0],
                   [
                     scrolled_window.hadjustment.value,
                     scrolled_window.vadjustment.value,
                   ])
    end

    test("vadjustment") do
      vadjustment = Gtk::Adjustment.new(50, 20, 80, 2, 20, 5)
      scrolled_window = Gtk::ScrolledWindow.new(nil, vadjustment)
      assert_equal([0, 50],
                   [
                     scrolled_window.hadjustment.value,
                     scrolled_window.vadjustment.value,
                   ])
    end
  end
end
