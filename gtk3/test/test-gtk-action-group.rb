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

class TestGtkActionGroup < Test::Unit::TestCase
  def setup
    @group = Gtk::ActionGroup.new("Ruby/GTK3")
  end

  def test_set_translate_func
    @group.set_translate_func do |message|
      "#{message} (translated)"
    end
    assert_equal("hello (translated)", @group.translate_string("hello"))
  end

  sub_test_case("#translate_string") do
    test("nil") do
      assert_nil(@group.translate_string(nil))
    end
  end

  sub_test_case("#add_action") do
    test("no options") do
      action = Gtk::Action.new("run")
      @group.add_action(action)
      assert_nil(action.accel_path)
    end

    test(":accelerator") do
      action = Gtk::Action.new("run")
      @group.add_action(action, :accelerator => "<Control>a")
      assert_equal("<Actions>/Ruby/GTK3/run", action.accel_path)
    end
  end
end
