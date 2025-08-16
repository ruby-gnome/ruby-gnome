# Copyright (C) 2025  Ruby-GNOME Project Team
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

module Gsk
  class RoundedRect
    class << self
      def try_convert(value)
        case value
        when Array
          n_values = value.size
          return nil if n_values < 2

          bounds = Graphene::Rect.try_convert(value[0])
          return nil if bounds.nil?
          case n_values
          when 2
            radius = value[1]
            new(bounds, radius)
          when 5
            corners = []
            (1..4).each do |i|
              corner = Graphene::Size.try_convert(value[i])
              return nil if corner.nil?
              corners << corner
            end
            new(bounds, *corners)
          else
            nil
          end
        else
          nil
        end
      end
    end

    alias_method :initialize_raw, :initialize
    def initialize(*args)
      super()
      case args.size
      when 0
      when 1
        init_copy(args[0])
      when 2
        init_from_rect(args[0], args[1])
      when 5
        init(*args)
      else
        message = +"wrong number of arguments "
        message << "(given #{args.size}, expected 0, 1, 2 or 5)"
        raise ArgumentError, message
      end
    end

    def ==(other)
      other.is_a?(self.class) and
        bounds == other.bounds and
        corners == other.corners
    end

    def inspect
      super.sub(/>\z/) do
        " bounds=#{bounds.inspect} corners=#{corners.inspect}>"
      end
    end
  end
end
