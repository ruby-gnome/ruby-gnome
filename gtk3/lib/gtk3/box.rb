# Copyright (C) 2014  Ruby-GNOME2 Project Team
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
  class Box
    alias_method :initialize_raw, :initialize
    def initialize(orientation, spacing=0)
      initialize_raw(orientation, spacing)
    end

    alias_method :pack_start_raw, :pack_start
    def pack_start(child, options={})
      expand  = options[:expand]  || false
      fill    = options[:fill]    || false
      padding = options[:padding] || 0
      pack_start_raw(child, expand, fill, padding)
    end
    
    alias_method :pack_end_raw, :pack_end
    def pack_end(child, options={})
      expand  = options[:expand]  || false
      fill    = options[:fill]    || false
      padding = options[:padding] || 0
      pack_end_raw(child, expand, fill, padding)
    end
  end
end
