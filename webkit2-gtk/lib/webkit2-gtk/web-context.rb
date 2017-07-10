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

module WebKit2Gtk
  class WebContext
    alias_method :initialize_raw, :initialize
    def initialize(*args)
      if args.size == 1 and args[0].is_a?(Hash)
        options = args[0]
        ephemeral = options[:ephemeral]
        if ephemeral
          initialize_new_ephemeral
        else
          initialize_raw
        end
      else
        initialize_raw(*args)
      end
    end
  end
end
