# Copyright (C) 2016  Ruby-GNOME2 Project Team
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

module Gtk
  class TextView
    def get_iter_at(options)
      location = options[:location]
      position = options[:position]

      if location
        get_iter_at_location(*location)
      elsif position
        get_iter_at_position(*position)
      else
        message = "must specify :location or :position: #{options.inspect}"
        raise ArgumentError, message
      end
    end

    alias_method :get_iter_at_location_raw, :get_iter_at_location
    def get_iter_at_location(x, y)
      _found, iter = get_iter_at_location_raw(x, y)
      iter
    end

    alias_method :get_iter_at_position_raw, :get_iter_at_position
    def get_iter_at_position(x, y)
      _found, iter, trailing = get_iter_at_position_raw(x, y)
      [iter, trailing]
    end
  end
end
