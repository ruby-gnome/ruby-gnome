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
    private_class_method(:from_hls)    if respond_to?(:from_hls)
    private_class_method(:from_pixel)  if respond_to?(:from_pixel)
    private_class_method(:from_string) if respond_to?(:from_string)

    class << self
      def new(*args)
        if [Symbol] == args.collect(&:class)
          name = args[0]
          get_static(name)
        else
          if args.empty?
            args = [0, 0, 0, 1] unless private_method_defined?(:initialize_alloc)
          end
          super(*args)
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
        if respond_to?(:from_hls, true)
          color = from_hls(hue, luminance, saturation)
        else
          color = new
          color.from_hls(hue, luminance, saturation)
        end
        color.alpha = alpha
        color
      end

      def pixel(pixel)
        if respond_to?(:from_pixel, true)
          color = from_pixel(pixel)
        else
          color = new
          color.from_pixel(pixel)
        end
        color
      end

      def parse(string)
        if respond_to?(:from_string, true)
          succeeded, color = from_string(string)
        else
          color = new
          succeeded = color.from_string(string)
        end
        unless succeeded
          raise ArgumentError, "invalid color string: <#{string}>"
        end
        color
      end
    end
  end
end
