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
  module CellLayout
    def set_attributes(cell, attributes)
      attributes.each do |key, value|
        add_attribute(cell, key, value)
      end
    end

    alias_method :add_attribute_raw, :add_attribute
    def add_attribute(cell, name, value)
      name = name.to_s if name.is_a?(Symbol)
      add_attribute_raw(cell, name, value)
    end
  end
end
