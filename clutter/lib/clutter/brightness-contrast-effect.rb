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

module Clutter
  class BrightnessContrastEffect
    alias_method :set_contrast_raw, :set_contrast
    def set_contrast(contrast_all_or_red, contrast_green=nil, contrast_blue=nil)
      if contrast_green.nil? && contrast_blue.nil?
        contrast_all = contrast_all_or_red
        set_contrast(contrast_all)
      else
        contrast_red = contrast_all_or_red
        set_contrast_full(contrast_red, contrast_green,
                          contrast_blue)
      end
    end

    alias_method :set_brightness_raw, :set_brightness
    def set_brightness(brightness_all_or_red, brightness_green=nil,
                       brightness_blue=nil)
      if brightness_green.nil? && brightness_blue.nil?
        brightness_all = brightness_all_or_red
        set_brightness(brightness_all)
      else
        brightness_red = brightness_all_or_red
        set_brightness_full(brightness_red, brightness_green,
                            brightness_blue)
      end
    end
  end
end
