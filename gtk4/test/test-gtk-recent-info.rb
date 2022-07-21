# Copyright (C) 2015-2022  Ruby-GNOME Project Team
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

class TestGtkRecentInfo < Test::Unit::TestCase
  include GtkTestUtils

  sub_test_case "instance methods" do
    def setup
      manager = Gtk::RecentManager.new
      uri = "file:///#{__FILE__}"
      manager.add_item(uri)
      loop = GLib::MainLoop.new
      GLib::Idle.add do
        loop.quit
        GLib::Source::REMOVE
      end
      loop.run
      @info = manager.lookup_item(uri)
    end

    test "#exist?" do
      assert do
        @info.exist?
      end
    end
  end
end
