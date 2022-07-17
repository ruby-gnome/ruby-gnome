# Copyright (C) 2018  Ruby-GNOME2 Project Team
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
  class Application
    alias_method :initialize_raw, :initialize
    def initialize(*args)
      case args.size
      when 0
        initialize_raw(nil, :flags_none)
      when 1
        if args[0].is_a?(String)
          initialize_raw(args[0], :flags_none)
        else
          initialize_raw(nil, args[0])
        end
      else
        initialize_raw(*args)
      end
    end
  end
end
