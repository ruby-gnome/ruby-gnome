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
      when FunctionInfo
        load_function_info(info)
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

    def load_function_info(info)
      @base_module.module_eval do
        define_method(info.name) do |*arguments|
          info.invoke(*arguments)
        end
        module_function(info.name)
      end
    end

    def load_struct_info(info)
      return if info.gtype_struct?
      return if info.gtype == GLib::Type::NONE

      klass = self.class.define_class(info.gtype, info.name, @base_module)
      load_field_infos(info, klass)
      info.methods.each do |method_info|
        load_method_info(method_info, klass)
      end
    end

    def load_enum_info(info)
      self.class.define_class(info.gtype, info.name, @base_module)
    end

    def load_object_info(info)
      klass = self.class.define_class(info.gtype, info.name, @base_module)
      load_field_infos(info, klass)
      info.methods.each do |method_info|
        load_method_info(method_info, klass)
      end
    end

    def load_field_infos(info, klass)
      info.n_fields.times do |i|
        field_info = info.get_field(i)
        name = field_info.name
        flags = field_info.flags

        if flags.readable?
          klass.__send__(:define_method, name) do
            info.get_field_value(self, i)
          end
        end

        if flags.writable?
          klass.__send__(:define_method, "#{name}=") do |value|
            info.set_field_value(self, i, value)
          end
        end
      end
    end

    def load_method_info(info, klass)
      name = info.name
      case info
      when ConstructorInfo
        if name == "new"
          klass.__send__(:define_method, "initialize") do |*arguments|
            info.invoke(self, *arguments)
          end
        else
          # TODO
        end
      when MethodInfo
        klass.__send__(:define_method, name) do |*arguments|
          info.invoke(self, *arguments)
        end
      else
        return if name == "new"
        return if name == "alloc"
        singleton_class = (class << klass; self; end)
        singleton_class.__send__(:define_method, name) do |*arguments|
          info.invoke(*arguments)
        end
      end
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
