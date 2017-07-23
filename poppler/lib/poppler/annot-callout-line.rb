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

module Poppler
  class AnnotCalloutLine
    alias_method :initialize_raw, :initialize
    def initialize(*args)
      initialize_raw
      if args.size == 1 && args.class == Hash
        self.multiline = args[:multiline] || false
        self.x1 = args[:x1] || 0
        self.y1 = args[:y1] || 0
        self.x2 = args[:x2] || 0
        self.y2 = args[:y2] || 0
        self.x3 = args[:x3] || 0
        self.y3 = args[:y3] || 0
      elsif args.size == 7
        self.multiline = args[0]
        self.x1 = args[1]
        self.y1 = args[2]
        self.x2 = args[3]
        self.y2 = args[4]
        self.x3 = args[5]
        self.y3 = args[6]
      end
    end

    def to_a
      [multiline?, x1, y1, x2, y2, x3, y3]
    end
  end
end
