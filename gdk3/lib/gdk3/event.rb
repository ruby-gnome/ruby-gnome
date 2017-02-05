# Copyright (C) 2014-2017  Ruby-GNOME2 Project Team
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
  class Event
    unless const_defined?(:PROPAGATE)
      PROPAGATE = false
    end
    unless const_defined?(:STOP)
      STOP = true
    end

    def send_event?
      not send_event.zero?
    end
  end

  class EventButton
    def click_count
      _, number = get_click_count
      number
    end
  end

  class EventFocus
    alias_method :in_raw=, :in=
    def in=(value)
      if value == true
        value = 1
      elsif !value
        value = 0
      end
      self.in_raw = value
    end

    alias_method :in_raw, :in
    remove_method :in
    def in?
      not in_raw.zero?
    end
  end

  class EventKey
    alias_method :keyval_raw, :keyval
    def keyval
      found, value = keyval_raw
      if found
        value
      else
        nil
      end
    end

    alias_method :keycode_raw, :keycode
    def keycode
      found, value = keycode_raw
      if found
        value
      else
        nil
      end
    end
  end

  class EventScroll
    def direction
      found, value = scroll_direction
      if found
        value
      else
        nil
      end
    end

    def deltas
      found, x, y = scroll_deltas
      if found
        [x, y]
      else
        nil
      end
    end

    if method_defined?(:scroll_stop_event?)
      alias_method :stop_event?, :scroll_stop_event?
    end
  end
end
