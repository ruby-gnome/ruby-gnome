# Copyright (C) 2016-2017  Ruby-GNOME2 Project Team
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
      result = get_iter_at_location_raw(x, y)
      if result.is_a?(Gtk::TextIter) # For GTK+ < 3.20
        iter = result
        iter
      else
        found, iter = result
        if found
          iter
        else
          nil
        end
      end
    end

    alias_method :get_iter_at_position_raw, :get_iter_at_position
    def get_iter_at_position(x, y)
      result = get_iter_at_position_raw(x, y)
      if result.size == 2 # For GTK+ < 3.20
        result
      else
        found, iter, trailing = result
        if found
          [iter, trailing]
        else
          nil
        end
      end
    end
  end
end
