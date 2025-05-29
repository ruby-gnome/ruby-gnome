# Copyright (C) 2015-2025  Ruby-GNOME Project Team
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

module Gio
  module Action
    alias_method :activate_raw, :activate
    def activate(parameter=nil)
      case parameter
      when nil, GLib::Variant
        # do nothing
      else
        parameter = GLib::Variant.new(parameter)
      end
      activate_raw(parameter)
    end

    alias_method :change_state_raw, :change_state
    def change_state(value)
      case value
      when GLib::Variant
        # do nothing
      else
        value = GLib::Variant.new(value, state_type)
      end
      change_state_raw(value)
    end
  end
end
