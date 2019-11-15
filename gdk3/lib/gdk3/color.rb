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
  class Color
    class << self
      alias_method :parse_raw, :parse
      def parse(spec)
        succeeded, color = parse_raw(spec)
        unless succeeded
          message = "Invalid color spec: <#{spec.inspect}>: "
          message << "Color spec must be one of them: "
          message << "\"\#rgb\", "
          message << "\"\#rrggbb\", "
          message << "\"\#rrggbb\", "
          message << "\"\#rrrgggbbb\", "
          message << "\"\#rrrrggggbbbb\""
          raise ArgumentError, message
        end
        color
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
    def initialize(red, green, blue)
      initialize_raw
      self.red = red
      self.green = green
      self.blue = blue
    end
  end
end
