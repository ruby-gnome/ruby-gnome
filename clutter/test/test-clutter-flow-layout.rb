# Copyright (C) 2014  Ruby-GNOME2 Project Team
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

class ClutterFlowLayoutTest < Test::Unit::TestCase
  include ClutterTestUtils

  def setup
    default_orientation = Clutter::FlowOrientation::VERTICAL
    @flow_layout = Clutter::FlowLayout.new(default_orientation)
  end

  def test_column_spacing_accessors
    column_space = 1.0
    @flow_layout.column_spacing = column_space
    assert_equal(column_space, @flow_layout.column_spacing)
  end

  def test_homogeneous_accessors
    @flow_layout.homogeneous = true
    assert_true(@flow_layout.homogeneous?)
  end

  def test_max_column_width_accessors
    max_width = 300
    @flow_layout.max_column_width = max_width
    assert_equal(max_width, @flow_layout.max_column_width)
  end

  def test_min_column_width_accessors
    min_width = 10
    @flow_layout.min_column_width = min_width
    assert_equal(min_width, @flow_layout.min_column_width)
  end

  def test_max_row_height_accessors
    max_height = 400
    @flow_layout.max_row_height = max_height
    assert_equal(max_height, @flow_layout.max_row_height)
  end

  def test_min_row_height_accessors
    min_height = 5
    @flow_layout.min_row_height = min_height
    assert_equal(min_height, @flow_layout.min_row_height)
  end

  def test_orientation_accessors
    changed_orientation = Clutter::FlowOrientation::HORIZONTAL
    @flow_layout.orientation = changed_orientation
    assert_equal(changed_orientation, @flow_layout.orientation)
  end

  def test_row_spacing_accessors
    row_space = 1.5
    @flow_layout.row_spacing = row_space
    assert_equal(row_space, @flow_layout.row_spacing)
  end

  def test_snap_to_grid_accessors
    only_clutter_version(1, 16, 0)
    @flow_layout.snap_to_grid = false
    assert_false(@flow_layout.snap_to_grid?)
  end

  class TestEnum < self
    def test_enum
      assert_const_defined(Clutter::FlowOrientation, :HORIZONTAL)
    end
  end
end
