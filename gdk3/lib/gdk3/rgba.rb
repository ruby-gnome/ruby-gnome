# Copyright (C) 2014-2019  Ruby-GNOME Project Team
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

module Gdk
  class RGBA
    class << self
      def parse(spec)
        rgba = new
        unless rgba.parse(spec)
          available_formats = [
            "COLOR_NAME",
            "\#RGB",
            "\#RRGGBB",
            "\#RRRGGGBBB",
            "\#RRRRGGGGBBBB",
            "rgb(R, G, B)",
            "rgba(R, G, B, A)",
          ]
          message = "invalid RGBA format: #{spec.inspect} "
          message << "(available formats: #{available_formats.join(', ')})"
          raise ArgumentError, message
        end
        rgba
      end

      def try_convert(value)
        case value
        when String
          parse(value)
        when Symbol
          parse(value.to_s)
        else
          nil
        end
      end
    end

    alias_method :initialize_raw, :initialize
    def initialize(red=nil, green=nil, blue=nil, alpha=nil)
      initialize_raw
      self.red = red if red
      self.green = green if green
      self.blue = blue if blue
      self.alpha = alpha || 1.0
    end

    def to_a
      [red, green, blue, alpha]
    end
  end
end
