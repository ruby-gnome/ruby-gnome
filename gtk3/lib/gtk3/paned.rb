# Copyright (C) 2015  Ruby-GNOME2 Project Team
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
  class Paned
    alias_method :pack1_raw, :pack1
    def pack1(child, options={})
      resize = options[:resize]
      resize = false if resize.nil?
      shrink = options[:shrink]
      shrink = true if shrink.nil?
      pack1_raw(child, resize, shrink)
    end

    alias_method :pack2_raw, :pack2
    def pack2(child, options={})
      resize = options[:resize]
      resize = true if resize.nil?
      shrink = options[:shrink]
      shrink = true if shrink.nil?
      pack2_raw(child, resize, shrink)
    end
  end
end
