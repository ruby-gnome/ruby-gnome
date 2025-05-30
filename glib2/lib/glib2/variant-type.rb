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
  class VariantType
    class << self
      def try_convert(value)
        case value
        when String
          new(value)
        when Symbol
          constant_name = value.to_s.upcase
          if const_defined?(constant_name)
            const_get(constant_name)
          else
            nil
          end
        else
          nil
        end
      end
    end

    def inspect
      super.gsub(/>\z/) do
        " string=#{self}>"
      end
    end
  end
end
