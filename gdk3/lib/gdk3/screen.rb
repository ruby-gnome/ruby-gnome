# Copyright (C) 2015-2018  Ruby-GNOME2 Project Team
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
  class Screen
    class << self
      alias_method :default_raw, :default
      def default
        @@default ||= default_raw
      end
    end

    alias_method :get_setting_raw, :get_setting
    def get_setting(key, type=nil)
      value = GLib::Value.new(type || GLib::Type::STRING)
      found = get_setting_raw(key, value)
      if found
        value
      else
        nil
      end
    end

    def get_monitor(window_or_x, y=nil)
      if window_or_x.is_a?(Window)
        window = window_or_x
        get_monitor_at_window(window)
      else
        x = window_or_x
        get_monitor_at_point(x, y)
      end
    end
  end
end
