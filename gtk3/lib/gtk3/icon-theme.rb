# Copyright (C) 2015  Ruby-GNOME2 Project Team
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
    alias_method :choose_icon_raw, :choose_icon
    def choose_icon(icon_names, size, flags=nil)
      icon_names = [icon_names] unless icon_names.is_a?(Array)
      flags ||= 0
      choose_icon_raw(icon_names, size, flags)
    end

    alias_method :lookup_icon_raw, :lookup_icon
    def lookup_icon(icon, size, flags=nil)
      case icon
      when String, Symbol
        flags ||= :generic_fallback
        lookup_icon_raw(icon.to_s, size, flags)
      else
        flags ||= 0
        lookup_by_gicon(icon, size, flags)
      end
    end
  end
end
