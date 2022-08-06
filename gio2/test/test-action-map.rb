# Copyright (C) 2015-2022  Ruby-GNOME2 Project Team
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


class TestActionMap < Test::Unit::TestCase
  def setup
    @map = Gio::SimpleActionGroup.new
  end

  sub_test_case "#add_actions" do
    test "name" do
      @map.add_actions([
                         {
                           :name => "open",
                         }
                       ])
      action = @map.lookup_action("open")
      assert_equal("open", action.name)
    end

    test "parameter_type" do
      args = nil
      callback = lambda do |*callback_args|
        args = callback_args
      end
      @map.add_actions([
                         {
                           :name => "open",
                           :parameter_type => "s",
                           :callback => callback,
                         }
                       ])
      action = @map.lookup_action("open")
      action.activate("X")
      assert_equal([action, "X"], args)
    end
  end

  def test_gc
    GC.start
    n_alive_actions_before = ObjectSpace.each_object(Gio::SimpleAction) {}
    n_tries = 100
    n_tries.times do |i|
      action_name = "test#{i}"
      action = Gio::SimpleAction.new(action_name)
      @map.add_action(action)
    end
    GC.start
    n_alive_actions_after = ObjectSpace.each_object(Gio::SimpleAction) {}
    n_alive_actions = n_alive_actions_after - n_alive_actions_before
    assert_equal(n_tries, n_alive_actions)
  end
end
