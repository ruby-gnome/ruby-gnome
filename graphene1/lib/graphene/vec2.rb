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

module Graphene
  class Vec2
    alias_method :initialize_raw, :initialize
    def initialize(*args)
      super()
      case args.size
      when 0
      when 1
        arg = args[0]
        case arg
        when Vec2
          init_from_vec2(arg)
        else
          message = +"source must be Graphene::Vec2: "
          message << "#{arg.inspect}"
          raise ArgumentError, message
        end
      when 2
        init(*args)
      else
        message = +"wrong number of arguments "
        message << "(given #{args.size}, expected 0..2)"
        raise ArgumentError, message
      end
    end

    def to_a
      [x, y]
    end
  end
end
