# Copyright (C) 2014  Ruby-GNOME2 Project Team
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
  class Window
    alias_method :initialize_raw, :initialize
    def initialize(type=:toplevel)
      if type.is_a?(String)
        initialize_raw(:toplevel)
        self.title = type
      else
        initialize_raw(type)
      end
    end

    alias_method :set_icon_raw, :set_icon
    def set_icon(icon_or_file_name)
      case icon_or_file_name
      when String
        set_icon_from_file(icon_or_file_name)
      else
        set_icon_raw(icon_or_file_name)
      end
    end

    remove_method :icon=
    alias_method :icon=, :set_icon
  end
end
