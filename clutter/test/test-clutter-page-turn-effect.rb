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

class ClutterPageTurnEffect < Test::Unit::TestCase
  include ClutterTestUtils

  def setup
    default_period = 1.0
    default_angle = 180.0
    default_radious = 20.0
    @page_turn_effect = Clutter::PageTurnEffect.new(default_period,
                                                    default_angle,
                                                    default_radious)
  end

  def test_angle_accessors
    page_angle = 150
    @page_turn_effect.angle = page_angle
    assert_equal(page_angle, @page_turn_effect.angle)
  end

  def test_period_accessors
    page_period = 0.5
    @page_turn_effect.period = page_period
    assert_equal(page_period, @page_turn_effect.period)
  end

  def test_radius_accessors
    page_radius = 50.0
    @page_turn_effect.radius = page_radius
    assert_equal(page_radius, @page_turn_effect.radius)
  end
end
