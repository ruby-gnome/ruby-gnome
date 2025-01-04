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

module GLib
  class Value
    class << self
      def try_convert(value)
        case value
        when String
          new(GLib::Type::STRING, value)
        when Integer
          if value.negative?
            if value < GLib::MININT32
              new(GLib::Type::INT64, value)
            else
              new(GLib::Type::INT, value)
            end
          else
            if value > GLib::MAXUINT32
              new(GLib::Type::UINT64, value)
            else
              new(GLib::Type::UINT, value)
            end
          end
        when Float
          new(GLib::Type::DOUBLE, value)
        else
          nil
        end
      end
    end
  end
end
