# Copyright (C) 2016  Ruby-GNOME2 Project Team
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

module GOffice 
  class Loader < GObjectIntrospection::Loader
    private
    def pre_load(repository, namespace)
    end

    def post_load(repository, namespace)
      require_libraries
    end

    def require_libraries
    end

    def load_enum_value(value_info, enum_module)
      name = case value_info.name.upcase
      when /\A30E_360\z/
        "EUR_30_360"
      when /\A30EP_360\z/
        "EP_30_360"
      else
        value_info.name.upcase
      end
      enum_module.const_set(name, value_info.value)
    end
    
    def load_object_info(info)
      name = rename_class(info.name)
      klass = self.class.define_class(info.gtype, name, @base_module)
      load_fields(info, klass)
      load_methods(info, klass)
    end

    def initialize_post(object)
      super
      return unless object.is_a?(GLib::Object)
      self.class.reference_gobject(object, :sink => true)
    end

    def load_enum_info(info)
      if info.gtype == GLib::Type::NONE
        enum_module = Module.new
        info.values.each do |value_info|
          load_enum_value(value_info, enum_module)
        end
        @base_module.const_set(rename_class(info.name), enum_module)
      else
        if info.error_domain
          define_error(info)
        else
          define_enum(info)
        end
      end
    end
    
    def load_struct_info(info)
      case info.name
      when /Class\z/
        base_class_name = $PREMATCH
        method_infos = info.methods.find_all do |method_info|
          method_info.is_a?(MethodInfo)
        end
        unless methods.empty?
          base_class = @base_module.const_get(rename_class(base_class_name))
          load_method_infos(method_infos, base_class.singleton_class)
        end
      else
        return if info.gtype_struct?

        define_struct(info)
      end
    end

    def define_struct(info, options={})
      name = rename_class(info.name)
      if info.gtype == GLib::Type::NONE
        klass = self.class.define_struct(info.size, name, @base_module,
                                         :parent => options[:parent])
      else
        klass = self.class.define_class(info.gtype, name, @base_module,
                                        :parent => options[:parent],
                                        :size   => info.size)
      end
      load_fields(info, klass)
      load_methods(info, klass)
    end
    
    def load_flags_info(info)
      if info.gtype == GLib::Type::NONE
        flags_module = Module.new
        info.values.each do |value_info|
          load_flag_value(value_info, flags_module)
        end
        @base_module.const_set(rename_class(info.name), flags_module)
      else
        self.class.define_class(info.gtype, flags_class_name(info), @base_module)
      end
    end

    def load_interface_info(info)
      interface_module =
        self.class.define_interface(info.gtype, rename_class(info.name), @base_module)
      load_methods(info, interface_module)
    end

    def load_constructor_infos(infos, klass)
      return if infos.empty?

      prepare = lambda do |info, method_name, arguments, &block|
        arguments, block = build_arguments(info, arguments, &block)
        validate_arguments(info, "#{klass}\##{method_name}", arguments)
        [arguments, block]
      end
      call_initialize_post = lambda do |object|
        initialize_post(object)
      end
      infos.each do |info|
        name = "initialize_#{info.name}"
        unlock_gvl = should_unlock_gvl?(info, klass)
        klass.__send__(:define_method, name) do |*arguments, &block|
          arguments, block = prepare.call(info, name, arguments, &block)
          info.invoke({
                        :receiver  => self,
                        :arguments => arguments,
                        :unlock_gvl => unlock_gvl,
                      },
                      &block)
          call_initialize_post.call(self)
        end
        klass.__send__(:private, name)
      end

      find_info = lambda do |arguments|
        find_suitable_callable_info(infos, arguments)
      end
      klass.__send__(:define_method, "initialize") do |*arguments, &block|
        info = find_info.call(arguments, &block)
        __send__("initialize_#{info.name}", *arguments, &block)
      end
    end
    
   def rename_class(class_name)
      name = case class_name
      when /\A3DRotationSel\z/
        "Rotation3DSel"
      when /\Ac(\d*[A-Z]\w*)\z/
        "Canvas#{Object::Regexp.last_match[0]}"
      when /\Ag(\d*[A-Z]\w*)\z/
        "Graph#{Object::Regexp.last_match[0]}"
      when /\Ago_regression_stat_t\z/
        "RegressionStatT"
      when /\Ago_regression_stat_tl\z/
        "RegressionStatTl"
      else
        class_name
      end
    end
  end
end
