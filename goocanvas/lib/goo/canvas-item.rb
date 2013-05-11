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

module Goo
  module CanvasItem
    alias_method :remove_child_raw, :remove_child
    private :remove_child_raw
    def remove_child(child_or_position)
      if child_or_position.is_a?(CanvasItem)
        child = child_or_position
        position = find_child(child)
      else
        position = child_or_position
        child = get_child(position)
      end
      remove_child_raw(position)
    end
  end
end
