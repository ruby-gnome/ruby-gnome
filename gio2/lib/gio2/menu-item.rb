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

module Gio
  class MenuItem
    alias_method :set_attribute_value_raw, :set_attribute_value
    def set_attribute_value(name, value)
      if !value.nil? and !value.is_a?(GLib::Variant)
        value = GLib::Variant.new(value)
      end
      set_attribute_value_raw(name, value)
    end

    alias_method :[]=, :set_attribute_value
    alias_method :[], :get_attribute_value

    Gio.constants.each do |constant_name|
      case constant_name.to_s
      when /\AMENU_ATTRIBUTE_/
        name = $POSTMATCH.downcase
        define_method(name) do
          self[Gio.const_get(constant_name)]
        end
        define_method("#{name}=") do |value|
          self[Gio.const_get(constant_name)] = value
        end
      end
    end
  end
end
