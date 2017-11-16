# Copyright (C) 2013  Ruby-GNOME2 Project Team
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

class TestGtkStyleProperties < Test::Unit::TestCase
  def setup
    @properties = Gtk::StyleProperties.new
  end

  class TestGet < self
    def test_existent
      key = "border-radius"
      state = :normal
      value = 29

      g_value = GLib::Value.new(GLib::Type::INT, value)
      # TODO: support direct Ruby object use
      @properties.set_property(key, state, g_value)
      assert_equal(value, @properties.get_property(key, state))
    end

    def test_nonexistent
      assert_nil(@properties.get_property("nonexistent", :normal))
    end
  end
end
