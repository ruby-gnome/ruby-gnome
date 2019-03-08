# Copyright (C) 2019  Ruby-GNOME2 Project Team
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

module GObjectIntrospection
  class TypeInfo
    def match?(value)
      case tag
      when TypeTag::BOOLEAN
        value == true or value == false
      when TypeTag::INT8,
           TypeTag::UINT8,
           TypeTag::INT16,
           TypeTag::UINT16,
           TypeTag::INT32,
           TypeTag::UINT32,
           TypeTag::INT64,
           TypeTag::UINT64,
           TypeTag::FLOAT,
           TypeTag::DOUBLE
        value.is_a?(Numeric)
      when TypeTag::GTYPE
        value.is_a?(GLib::Type)
      when TypeTag::UTF8
        value.is_a?(String)
      when TypeTag::FILENAME
        value.is_a?(String)
      when TypeTag::ARRAY
        element_type_info = get_param_type(0)
        value.is_a?(Array) and value.all? {|v| element_type_info.match?(v)}
      when TypeTag::INTERFACE
        interface.match?(value)
      when TypeTag::GLIST,
           TypeTag::GSLIST
        element_type_info = get_param_type(0)
        value.is_a?(Array) and value.all? {|v| element_type_info.match?(v)}
      else
        # TODO
        false
      end
    end
  end
end
