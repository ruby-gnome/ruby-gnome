# Copyright (C) 2022 Ruby-GNOME2 Project Team
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

class TestApplication < Test::Unit::TestCase
  def setup
    @app = Gio::Application.new("org.example.test", 0)
  end

  sub_test_case "as Gio::ActionMap" do
    def setup
      super
      @action_name = "test"
      @action = Gio::SimpleAction.new(@action_name)
    end

    test "add action, lookup action" do
      @app.add_action(@action)
      out = @app.lookup_action(@action_name)
      assert_equal(@action, out)
    end

    test "add action, remove action, lookup action" do
      @app.add_action(@action)
      @app.remove_action(@action_name)
      out = @app.lookup_action(@action_name)
      assert_nil(out)
    end

    test "add action, GC, lookup action" do
      @app.add_action(@action)
      GC.start
      out = @app.lookup_action(@action_name)
      assert_equal(@action, out)
    end
  end
end
