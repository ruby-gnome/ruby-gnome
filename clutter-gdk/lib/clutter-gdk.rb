# Copyright (C) 2016  Ruby-GNOME2 Project Team
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

require "clutter"
require "gdk3"

module ClutterGdk
  loader = GObjectIntrospection::Loader.new(self)
  loader.load("ClutterGdk")
end

init_clutter = lambda do
  module Clutter
    class Stage
      def gdk_window
        ClutterGdk.get_stage_window(self)
      end

      def set_foreign_window(window)
        ClutterGdk.set_stage_foreign(self, window)
      end
      alias_method :foreign_window=, :set_foreign_window
    end
  end
end

if Clutter.respond_to?(:init)
  Clutter.on_init do
    init_clutter.call
  end
else
  init_clutter.call
end

init_gdk = lambda do
  module Gdk
    class Window
      def clutter_stage
        ClutterGdk.get_stage_from_window(self)
      end
    end
  end
end

if Gdk.respond_to?(:init)
  Gdk.on_init do
    init_gdk.call
  end
else
  init_gdk.call
end
