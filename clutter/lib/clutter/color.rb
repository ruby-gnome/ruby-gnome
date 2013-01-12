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

module Clutter
  class Color
    class << self
      def new(*args)
        if [Symbol] == args.collect(&:class)
          name = args[0]
          get_static(name)
        else
          super
        end
      end

      def rgb(red, green, blue, alpha=255)
        rgba(red, green, blue, alpha)
      end

      def rgba(red, green, blue, alpha)
        new(red, green, blue, alpha)
      end

      def hls(hue, luminance, saturation, alpha=255)
        hlsa(hue, luminance, saturation, alpha)
      end

      def hlsa(hue, luminance, saturation, alpha)
        color = new
        color.from_hls(hue, luminance, saturation)
        color.alpha = alpha
        color
      end

      def pixel(pixel)
        color = new
        color.from_pixel(pixel)
        color
      end
    end
  end
end
