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

class TestGtkCellLayout < Test::Unit::TestCase
  include GtkTestUtils

  def setup
    @layout = Gtk::ComboBox.new
    @renderer = Gtk::CellRendererText.new
    @layout.pack_start(@renderer, false)
  end

  sub_test_case "#set_attributes" do
    test "empty" do
      @layout.set_attributes(@renderer, {})
    end

    test "multiple" do
      @layout.set_attributes(@renderer, {:text => 1})
    end
  end

  test "#set_cell_data_func" do
    @layout.set_cell_data_func(@renderer) do
      # TODO: assert block arguments
    end
  end
end
