# Copyright (C) 2019  Ruby-GNOME Project Team
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
      nil
    end

    def description(type_info)
      nick
    end

    class << BOOLEAN
      def try_convert(type_info, value)
        case value
        when true, false
          value
        when nil
          false
        else
          nil
        end
      end
    end

    module IntegerTypeTag
      def try_convert(type_info, value)
        if value.is_a?(Integer)
          value
        elsif value.respond_to?(:to_int)
          value.to_int
        else
          nil
        end
      end
    end

    INT8.extend(IntegerTypeTag)
    UINT8.extend(IntegerTypeTag)
    INT16.extend(IntegerTypeTag)
    UINT16.extend(IntegerTypeTag)
    INT32.extend(IntegerTypeTag)
    UINT32.extend(IntegerTypeTag)
    INT64.extend(IntegerTypeTag)
    UINT64.extend(IntegerTypeTag)

    module FloatTypeTag
      def try_convert(type_info, value)
        if value.is_a?(Float)
          value
        elsif value.respond_to?(:to_f) # TODO: Should we stop this?
          value.to_f
        else
          nil
        end
      end
    end

    FLOAT.extend(FloatTypeTag)
    DOUBLE.extend(FloatTypeTag)

    class << GTYPE
      def try_convert(type_info, value)
        GLib::Type.try_convert(value)
      end
    end

    class << UTF8
      def try_convert(type_info, value)
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
    end

    class << FILENAME
      def try_convert(type_info, value)
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
    end

    module ArrayTypeTag
      def try_convert(type_info, value)
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

      def description(type_info)
        element_type_info = type_info.get_param_type(0)
        "#{super}(#{element_type_info.description})"
      end

      def get_element_type_info(type_info)
        type_info.get_param_type(0)
      end
    end

    class << ARRAY
      include ArrayTypeTag

      def try_convert(type_info, value)
        case get_element_type_info(type_info).tag
        when INT8, UINT8
          case value
          when String
            return value
          when GLib::Bytes
            return value.to_s
          end
        end
        super
      end
    end

    GLIST.extend(ArrayTypeTag)
    GSLIST.extend(ArrayTypeTag)

    class << GHASH
      def try_convert(type_info, value)
        case value
        when Hash
          value
        else
          nil
        end
      end

      def description(type_info)
        key_type = type_info.get_param_type(0)
        value_type = type_info.get_param_type(1)
        "#{super}(#{key_type.description}->#{value_type.description})"
      end
    end

    class << INTERFACE
      def try_convert(type_info, value)
        type_info.interface.try_convert(value)
      end

      def description(type_info)
        "#{super}(#{type_info.interface.description})"
      end
    end
  end
end
