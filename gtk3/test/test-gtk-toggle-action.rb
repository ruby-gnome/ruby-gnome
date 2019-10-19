# Copyright (C) 2015-2019  Ruby-GNOME Project Team
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

class TestGtkToggleAction < Test::Unit::TestCase
  include GtkTestUtils

  sub_test_case(".new") do
    test("no options") do
      action = Gtk::ToggleAction.new("run")
      assert_equal("run", action.name)
    end

    test(":label") do
      action = Gtk::ToggleAction.new("run", :label => "Run")
      assert_equal("Run", action.label)
    end

    test(":tooltip") do
      action = Gtk::ToggleAction.new("run", :tooltip => "Run this action")
      assert_equal("Run this action", action.tooltip)
    end

    test(":stock_id") do
      action = Gtk::ToggleAction.new("add", :stock_id => "gtk-add")
      assert_equal("gtk-add", action.stock_id)
    end
  end

  sub_test_case("instance methods") do
    def setup
      @action = Gtk::ToggleAction.new("add")
    end

    sub_test_case("#active=") do
      def test_nil
        @action.active = nil
        assert do
          not @action.active?
        end
      end

      def test_true
        @action.active = true
        assert do
          @action.active?
        end
      end

      def test_false
        @action.active = false
        assert do
          not @action.active?
        end
      end
    end
  end
end
