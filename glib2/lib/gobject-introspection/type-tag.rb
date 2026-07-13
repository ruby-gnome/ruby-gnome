# Copyright (C) 2019-2021  Ruby-GNOME Project Team
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
  class TypeTag
    def try_convert(type_info, value)
      method = "try_convert_#{nick}"
      if respond_to?(method, true)
        __send__(method, type_info, value)
      else
        nil
      end
    end

    def description(type_info)
      method = "description_#{nick}"
      if respond_to?(method, true)
        __send__(method, type_info)
      else
        nick
      end
    end

    private
    def try_convert_boolean(type_info, value)
      case value
      when true, false
        value
      when nil
        false
      else
        nil
      end
    end

    def try_convert_integer(type_info, value)
      if value.is_a?(Integer)
        value
      elsif value.respond_to?(:to_int)
        value.to_int
      else
        nil
      end
    end

    alias_method :try_convert_int8, :try_convert_integer
    alias_method :try_convert_uint8, :try_convert_integer
    alias_method :try_convert_int16, :try_convert_integer
    alias_method :try_convert_uint16, :try_convert_integer
    alias_method :try_convert_int32, :try_convert_integer
    alias_method :try_convert_uint32, :try_convert_integer
    alias_method :try_convert_int64, :try_convert_integer
    alias_method :try_convert_uint64, :try_convert_integer

    def try_convert_float(type_info, value)
      if value.is_a?(Float)
        value
      elsif value.respond_to?(:to_f) # TODO: Should we stop this?
        value.to_f
      else
        nil
      end
    end

    alias_method :try_convert_double, :try_convert_float

    def try_convert_gtype(type_info, value)
      GLib::Type.try_convert(value)
    end

    def try_convert_utf8(type_info, value)
      case value
      when String
        value.encode(Encoding::UTF_8)
      when Symbol
        value.to_s.encode(Encoding::UTF_8)
      else
        if value.respond_to?(:to_str)
          value.to_str.encode(Encoding::UTF_8)
        else
          nil
        end
      end
    end

    def try_convert_filename(type_info, value)
      case value
      when String
        value.encode(GLib::FILENAME_ENCODING)
      else
        if value.respond_to?(:to_path)
          value.to_path.encode(GLib::FILENAME_ENCODING)
        elsif value.respond_to?(:to_str)
          value.to_str.encode(GLib::FILENAME_ENCODING)
        else
          nil
        end
      end
    end

    def try_convert_array_like(type_info, value)
      value = Array.try_convert(value)
      return nil if value.nil?
      element_type_info = get_element_type_info(type_info)
      value.collect do |v|
        unless v.nil?
          v = element_type_info.try_convert(v)
          return nil if v.nil?
        end
        v
      end
    end

    def description_array_like(type_info)
      element_type_info = type_info.get_param_type(0)
      "#{nick}(#{element_type_info.description})"
    end

    def get_element_type_info(type_info)
      type_info.get_param_type(0)
    end

    def try_convert_array(type_info, value)
      case get_element_type_info(type_info).tag
      when INT8, UINT8
        case value
        when String
          return value
        when GLib::Bytes
          return value.to_s
        end
      end
      try_convert_array_like(type_info, value)
    end

    alias_method :description_array, :description_array_like

    alias_method :try_convert_glist, :try_convert_array_like
    alias_method :description_glist, :description_array_like
    alias_method :try_convert_gslist, :try_convert_array_like
    alias_method :description_gslist, :description_array_like

    def try_convert_ghash(type_info, value)
      case value
      when Hash
        value
      else
        nil
      end
    end

    def description_ghash(type_info)
      key_type = type_info.get_param_type(0)
      value_type = type_info.get_param_type(1)
      "#{nick}(#{key_type.description}->#{value_type.description})"
    end

    def try_convert_interface(type_info, value)
      type_info.interface.try_convert(value)
    end

    def description_interface(type_info)
      "#{nick}(#{type_info.interface.description})"
    end
  end
end
