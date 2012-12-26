# Copyright (C) 2012  Ruby-GNOME2 Project Team
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
  class Loader
    class << self
      def load(namespace, base_module)
        loader = new(base_module)
        loader.load(namespace)
      end
    end

    def initialize(base_module)
      @base_module = base_module
    end

    def load(namespace)
      repository = Repository.default
      repository.require(namespace)
      repository.each(namespace) do |info|
        load_info(info)
      end
    end

    private
    def load_info(info)
      case info
      when StructInfo
        load_struct_info(info)
      when EnumInfo
        load_enum_info(info)
      when ObjectInfo
        load_object_info(info)
      when InterfaceInfo
        load_interface_info(info)
      when ConstantInfo
        load_constant_info(info)
      when UnionInfo
        load_union_info(info)
      end
    end

    def load_struct_info(info)
      # TODO
    end

    def load_enum_info(info)
      self.class.define_class(info.gtype, info.name, @base_module)
    end

    def load_object_info(info)
      self.class.define_class(info.gtype, info.name, @base_module)
    end

    def load_interface_info(info)
      self.class.define_interface(info.gtype, info.name, @base_module)
    end

    def load_constant_info(info)
      @base_module.const_set(info.name, info.value)
    end

    def load_union_info(info)
      # TODO
    end
  end
end
