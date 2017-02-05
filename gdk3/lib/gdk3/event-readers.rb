# Copyright (C) 2017  Ruby-GNOME2 Project Team
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
  module EventAxisReader
    def get_axis(type)
      found, value = super(type)
      if found
        value
      else
        nil
      end
    end
  end

  module EventButtonReader
    def button
      found, value = super
      if found
        value
      else
        nil
      end
    end
  end

  module EventStateReader
    def state
      found, value = super
      if found
        value
      else
        nil
      end
    end
  end

  module EventCoordsReader
    def coords
      found, x, y = super
      if found
        [x, y]
      else
        nil
      end
    end

    def x
      (coords || [])[0]
    end

    def y
      (coords || [])[1]
    end

    def get_distance(other_event)
      calculated, distance = Event.get_distance(self, other_event)
      if calculated
        distance
      else
        nil
      end
    end

    def get_angle(other_event)
      calculated, angle = Event.get_angle(self, other_event)
      if calculated
        angle
      else
        nil
      end
    end

    def center
      calculated, x, y = super
      if calculated
        [x, y]
      else
        nil
      end
    end
  end

  module EventRootCoordsReader
    def root_coords
      found, x, y = super
      if found
        [x, y]
      else
        nil
      end
    end

    def x_root
      (root_coords || [])[0]
    end
    alias_method :root_x, :x_root

    def y_root
      (root_coords || [])[1]
    end
    alias_method :root_y, :y_root
  end
end
