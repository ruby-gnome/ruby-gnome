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

class ClutterBrightnessContrastEffectTest < Test::Unit::TestCase
  include ClutterTestUtils

  def setup
    @effect = Clutter::BrightnessContrastEffect.new
  end

  def test_brightness
    brightness = 127
    @effect.brightness = brightness
    assert_equal([brightness, brightness, brightness],
                 @effect.brightness)

    brightness_red = 120
    brightness_green = 200
    brightness_blue = 250
    @effect.set_brightness(brightness_red, brightness_green,
                           brightness_blue)
    assert_equal([brightness_red, brightness_green, brightness_blue],
                 @effect.brightness)
  end

  def test_contrast
    contrast = 127
    @effect.contrast = contrast
    assert_equal([contrast, contrast, contrast],
                 @effect.contrast)

    contrast_red = 250
    contrast_green = 120
    contrast_blue = 100
    @effect.set_contrast(contrast_red, contrast_green, contrast_blue)
    assert_equal([contrast_red, contrast_green, contrast_blue],
                 @effect.contrast)
  end
end
