# Copyright (C) 2014-2015  Ruby-GNOME2 Project Team
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
  class Window
    alias_method :invalidate, :invalidate_rect

    alias_method :user_data_raw, :user_data
    def user_data
      Loader.instantiate_gobject_pointer(user_data_raw)
    end

    alias_method :origin_raw, :origin
    def origin
      _, x, y = origin_raw
      [x, y]
    end
  end
end
