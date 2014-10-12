# Copyright (C) 2014 Ruby-GNOME2 Project Team
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

class TestGtkFlowBox < Test::Unit::TestCase
  include GtkTestUtils

  def setup
    only_gtk_version(3, 12, 0)
    @flow_box = Gtk::FlowBox.new
  end

  def test_homogeneous_accessors
    @flow_box.homogeneous = true
    assert_true(@flow_box.homogeneous?)
  end

  def test_row_spaceing_accessors
    row_spaces = 10
    @flow_box.row_spacing = row_spaces
    assert_equal(row_spaces, @flow_box.row_spacing)
  end

  def test_column_spacing_accessors
    column_spaces = 10
    @flow_box.column_spacing = column_spaces
    assert_equal(column_spaces, @flow_box.column_spacing)
  end

  def test_min_children_per_line_accessors
    min_value = 3
    @flow_box.min_children_per_line = min_value
    assert_equal(min_value, @flow_box.min_children_per_line)
  end

  def test_max_children_per_line_accessors
    max_value = 10
    @flow_box.max_children_per_line = max_value
    assert_equal(max_value, @flow_box.max_children_per_line)
  end

  def test_activate_single_on_click_accessors
    @flow_box.activate_on_single_click = false
    assert_false(@flow_box.activate_on_single_click?)
  end

  def test_selection_mode_accessors
    selection_browse = Gtk::SelectionMode::BROWSE
    @flow_box.selection_mode = selection_browse
    assert_equal(selection_browse, @flow_box.selection_mode)
  end
end
