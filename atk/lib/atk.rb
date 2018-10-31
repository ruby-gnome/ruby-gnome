# Copyright (C) 2017-2018  Ruby-GNOME2 Project Team
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

require "gobject-introspection"

module Atk
  LOG_DOMAIN = "Atk"
  GLib::Log.set_log_domain(LOG_DOMAIN)

  class Loader < GObjectIntrospection::Loader
  end

  loader = Loader.new(self)
  loader.load("Atk")

  class TextRectangle
    def initialize(x=nil, y=nil, width=nil, height=nil)
      super()
      self.x = x if x
      self.y = y if y
      self.width = width if width
      self.height = height if height
    end
  end

  module Version
    MAJOR = MAJOR_VERSION
    MINOR = MINOR_VERSION
    MICRO = MICRO_VERSION
    STRING = "#{MAJOR_VERSION}.#{MINOR_VERSION}.#{MICRO_VERSION}"
    class << self
      def or_later?(major, minor, micro=nil)
        micro ||= 0
        version = [
          MAJOR_VERSION,
          MINOR_VERSION,
          MICRO_VERSION,
        ]
        (version <=> [major, minor, micro]) >= 0
      end
    end
  end
end
