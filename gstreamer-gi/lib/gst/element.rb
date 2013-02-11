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

module Gst
  class Element
    def stop
      set_state(State::NULL)
    end

    def ready
      set_state(State::READY)
    end

    def pause
      set_state(State::PAUSED)
    end

    def play
      set_state(State::PLAYING)
    end

    alias_method :link_element, :link
    def link_elements(*elements)
      elements.each do |element|
        link_element(element)
      end
    end
    alias_method :link, :link_elements

    def >>(element)
      link_element(element)
      element
    end
  end
end
