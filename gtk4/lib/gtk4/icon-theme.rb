# Copyright (C) 2015-2022  Ruby-GNOME Project Team
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
  class IconTheme
    alias_method :lookup_icon_raw, :lookup_icon
    def lookup_icon(icon, size, scale: 1, direction: :none, flags: nil)
      case icon
      when String, Symbol
        flags ||= 0
        lookup_icon_raw(icon.to_s, nil, size, scale, direction, flags)
      when Array
        icon, *fallbacks = *icon
        flags ||= 0
        lookup_icon_raw(icon.to_s, fallbacks, size, scale, direction, flags)
      else
        flags ||= 0
        lookup_by_gicon(icon, size, scale, direction, flags)
      end
    end
  end
end
