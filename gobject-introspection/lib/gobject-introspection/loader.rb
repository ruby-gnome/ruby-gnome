# Copyright (C) 2012-2015  Ruby-GNOME2 Project Team
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

require "English"

module GObjectIntrospection
  class Loader
    class << self
      def load(namespace, base_module, options={})
        loader = new(base_module)
        loader.version = options[:version]
        loader.load(namespace)
      end
    end

    attr_accessor :version
    def initialize(base_module)
      @base_module = base_module
      @version = nil
    end

    def load(namespace)
      repository = Repository.default
      repository.require(namespace, @version)
      pre_load(repository, namespace)
      repository.each(namespace) do |info|
        load_info(info)
      end
      post_load(repository, namespace)
    end

    private
    def pre_load(repository, namespace)
    end

    def post_load(repository, namespace)
    end

    def load_info(info)
      case info
      when FunctionInfo
        load_function_info(info)
      when StructInfo
        load_struct_info(info)
      when BoxedInfo
        load_boxed_info(info)
      when FlagsInfo
        load_flags_info(info)
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
      name = rubyish_method_name(info)
      define_singleton_method(@base_module, name, info)
    end

    def define_module_function(target_module, name, function_info)
      unlock_gvl = should_unlock_gvl?(function_info, target_module)
      prepare = lambda do |arguments, &block|
        arguments, block = build_arguments(function_info, arguments, &block)
        method_name = "#{target_module}\#.#{name}"
        validate_arguments(function_info, method_name, arguments)
        [arguments, block]
      end
      target_module.module_eval do
        define_method(name) do |*arguments, &block|
          arguments, block = prepare.call(arguments, &block)
          function_info.invoke({
                                 :arguments => arguments,
                                 :unlock_gvl => unlock_gvl,
                               },
                               &block)
        end
        module_function(name)
      end
    end

    def define_singleton_method(klass, name, info)
      unlock_gvl = should_unlock_gvl?(info, klass)
      prepare = lambda do |arguments|
        arguments, block = build_arguments(info, arguments, &block)
        validate_arguments(info, "#{klass}.#{name}", arguments)
        [arguments, block]
      end
      singleton_class = (class << klass; self; end)
      singleton_class.__send__(:define_method, name) do |*arguments, &block|
        arguments, block = prepare.call(arguments, &block)
        if block.nil? and info.require_callback?
          to_enum(name, *arguments)
        else
          info.invoke({
                        :arguments => arguments,
                        :unlock_gvl => unlock_gvl,
                      },
                      &block)
        end
      end
    end

    def define_struct(info, options={})
      if info.gtype == GLib::Type::NONE
        klass = self.class.define_struct(info.size, info.name, @base_module,
                                         :parent => options[:parent])
      else
        klass = self.class.define_class(info.gtype, info.name, @base_module,
                                        :parent => options[:parent],
                                        :size   => info.size)
      end
      load_fields(info, klass)
      load_methods(info, klass)
    end

    def load_struct_info(info)
      case info.name
      when /Class\z/
        base_class_name = $PREMATCH
        method_infos = info.methods.find_all do |method_info|
          method_info.is_a?(MethodInfo)
        end
        unless methods.empty?
          base_class = @base_module.const_get(base_class_name)
          load_method_infos(method_infos, base_class.singleton_class)
        end
      else
        return if info.gtype_struct?

        define_struct(info)
      end
    end

    def define_boxed(info)
      klass = self.class.define_class(info.gtype, info.name, @base_module)
      _ = klass # TODO: Remove me. It is just for suppressing a warning.
      # TODO
      # load_fields(info, klass)
      # load_methods(info, klass)
    end

    def load_boxed_info(info)
      define_boxed(info)
    end

    def load_enum_value(value_info, enum_module)
      enum_module.const_set(value_info.name.upcase, value_info.value)
    end

    def define_enum(info)
      self.class.define_class(info.gtype,
                              enum_class_name(info),
                              @base_module)
    end

    def enum_class_name(info)
      info.name
    end

    def define_error(info)
      self.class.define_error(info.error_domain,
                              error_class_name(info),
                              @base_module,
                              :parent => error_parent_class(info),
                              :gtype => info.gtype)
    end

    def error_class_name(info)
      info.name
    end

    def error_parent_class(info)
      nil
    end

    def load_enum_info(info)
      if info.gtype == GLib::Type::NONE
        enum_module = Module.new
        info.values.each do |value_info|
          load_enum_value(value_info, enum_module)
        end
        @base_module.const_set(info.name, enum_module)
      else
        if info.error_domain
          define_error(info)
        else
          define_enum(info)
        end
      end
    end

    def load_flag_value(value_info, flags_module)
      flags_module.const_set(value_info.name.upcase, value_info.value)
    end

    def load_flags_info(info)
      if info.gtype == GLib::Type::NONE
        flags_module = Module.new
        info.values.each do |value_info|
          load_flag_value(value_info, flags_module)
        end
        @base_module.const_set(info.name, flags_module)
      else
        self.class.define_class(info.gtype, flags_class_name(info), @base_module)
      end
    end

    def flags_class_name(info)
      info.name
    end

    def load_object_info(info)
      klass = self.class.define_class(info.gtype, info.name, @base_module)
      load_fields(info, klass)
      load_methods(info, klass)
    end

    def load_fields(info, klass)
      info.n_fields.times do |i|
        field_info = info.get_field(i)
        load_field(info, i, field_info, klass)
      end
    end

    def field_name(field_info, klass)
      field_info.name
    end

    def load_field(info, i, field_info, klass)
      name = field_name(field_info, klass)
      flags = field_info.flags

      if flags.readable?
        if field_info.type.tag == TypeTag::BOOLEAN
          reader_method_name = "#{name}?"
        else
          reader_method_name = name
        end
        remove_existing_method(klass, reader_method_name)
        klass.__send__(:define_method, reader_method_name) do ||
          info.get_field_value(self, i)
        end
      end

      if flags.writable?
        klass.__send__(:define_method, "#{name}=") do |value|
          info.set_field_value(self, i, value)
        end
      end
    end

    def load_methods(info, klass)
      grouped_methods = info.methods.group_by do |method_info|
        method_info.class
      end
      grouped_methods.each do |method_info_class, method_infos|
        next if method_infos.empty?
        case method_infos.first
        when ConstructorInfo
          load_constructor_infos(method_infos, klass)
        when MethodInfo
          load_method_infos(method_infos, klass)
        when FunctionInfo
          load_function_infos(method_infos, klass)
        else
          raise "TODO: #{method_info_class}"
        end
      end
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

    def initialize_post(object)
    end

    def build_arguments(info, arguments, &block)
      last_in_arg = info.in_args.last
      if block and last_in_arg and last_in_arg.gclosure?
        [arguments + [block], nil]
      else
        [arguments, block]
      end
    end

    def validate_arguments(info, method_name, arguments)
      n_in_args = info.n_in_args
      n_required_in_args = info.n_required_in_args
      return if (n_required_in_args..n_in_args).cover?(arguments.size)

      detail = "#{arguments.size} for "
      if n_in_args == n_required_in_args
        detail << "#{n_in_args}"
      else
        detail << "#{n_required_in_args}..#{n_in_args}"
      end
      message = "#{method_name}: wrong number of arguments (#{detail})"
      raise ArgumentError, message
    end

    def find_suitable_callable_info(infos, arguments)
      min_n_args = nil
      max_n_args = nil
      candidate_infos = []
      infos.each do |info|
        n_in_args = info.n_in_args
        n_required_in_args = info.n_required_in_args
        if (n_required_in_args..n_in_args).cover?(arguments.size)
          candidate_infos << info
        end
        min_n_args = [min_n_args || n_required_in_args, n_required_in_args].min
        max_n_args = [max_n_args || n_in_args, n_in_args].max
      end

      if candidate_infos.size == 1
        return candidate_infos.first
      elsif candidate_infos.size > 1
        candidate_info = candidate_infos.find do |info|
          in_arg_infos = info.in_args
          arguments.each.with_index.all? do |argument, i|
            match_argument?(in_arg_infos[i], argument)
          end
        end
        return candidate_info || candidate_infos.first
      end

      detail = "#{arguments.size} for #{min_n_args}"
      if min_n_args < max_n_args
        detail << "..#{max_n_args}"
      end
      raise ArgumentError, "wrong number of arguments (#{detail})"
    end

    def match_argument?(arg_info, argument)
      type = arg_info.type
      case type.tag
      when TypeTag::BOOLEAN
        argument == true or argument == false
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
        argument.is_a?(Numeric)
      when TypeTag::GTYPE
        argument.is_a?(GLib::Type)
      when TypeTag::UTF8
        argument.is_a?(String)
      when TypeTag::FILENAME
        argument.is_a?(String)
      when TypeTag::ARRAY
        argument.is_a?(Array)
      when TypeTag::INTERFACE
        interface = type.interface
        case interface.type
        when InfoType::STRUCT
          match_argument_interface_struct?(arg_info, interface, argument)
        when InfoType::OBJECT,
             InfoType::INTERFACE
          argument.is_a?(interface.gtype.to_class)
        else
          # TODO
          false
        end
      when TypeTag::GLIST,
           TypeTag::GSLIST
        argument.is_a?(Array)
      else
        # TODO
        false
      end
    end

    def match_argument_interface_struct?(arg_info, interface, argument)
      gtype = interface.gtype
      case gtype.name
      when "void"
        # TODO
        false
      when "CairoSurface"
        if Object.const_defined?(:Cairo)
          argument.is_a?(Cairo::Surface)
        else
          false
        end
      else
        argument.is_a?(gtype.to_class)
      end
    end

    def rubyish_method_name(function_info, options={})
      name = function_info.name
      if options[:prefix]
        name = name.gsub(/\A#{Regexp.escape(options[:prefix])}/, "")
      end
      return_type = function_info.return_type
      return_type_tag = return_type.tag
      case return_type_tag
      when TypeTag::BOOLEAN
        case name
        when /\A(?:is|get_is)_/
          "#{$POSTMATCH}?"
        when /\Aget_/
          if function_info.n_in_args.zero?
            if function_info.n_out_args.zero?
              "#{$POSTMATCH}?"
            else
              $POSTMATCH
            end
          else
            name
          end
        when /\A(?:has|use|can)_/
          "#{name}?"
        else
          name
        end
      when TypeTag::GLIST, TypeTag::GSLIST
        case name
        when /\A(?:list|get)_/
          if function_info.n_in_args.zero?
            $POSTMATCH
          else
            name
          end
        else
          name
        end
      else
        case name
        when /\Aget_/
          if function_info.n_in_args.zero?
            $POSTMATCH
          else
            name
          end
        when "to_string"
          "to_s"
        when "foreach"
          "each"
        else
          name
        end
      end
    end

    def should_unlock_gvl?(function_info, klass)
      false
    end

    def load_method_infos(infos, klass)
      infos.each do |info|
        method_name = rubyish_method_name(info)
        load_method_info(info, klass, method_name)
      end
    end

    def load_method_info(info, klass, method_name)
      define_method(info, klass, method_name)
      define_equal_style_setter(info, klass, method_name)
    end

    def remove_existing_method(klass, method_name)
      return unless klass.method_defined?(method_name)
      return unless klass.instance_method(method_name).owner == klass
      klass.__send__(:remove_method, method_name)
    end

    def define_method(info, klass, method_name)
      unlock_gvl = should_unlock_gvl?(info, klass)
      remove_existing_method(klass, method_name)
      function_info_p = (info.class == FunctionInfo)
      no_return_value_p =
        (info.return_type.tag == TypeTag::VOID and info.n_out_args.zero?)
      prepare = lambda do |arguments, &block|
        arguments, block = build_arguments(info, arguments, &block)
        validate_arguments(info, "#{klass}\##{method_name}", arguments)
        [arguments, block]
      end
      klass.__send__(:define_method, method_name) do |*arguments, &block|
        arguments = [self] + arguments if function_info_p
        arguments, block = prepare.call(arguments, &block)
        if block.nil? and info.require_callback?
          to_enum(method_name, *arguments)
        else
          options = {
            :arguments => arguments,
            :unlock_gvl => unlock_gvl,
          }
          options[:receiver] = self unless function_info_p
          return_value = info.invoke(options, &block)
          if no_return_value_p
            self
          else
            return_value
          end
        end
      end
    end

    def define_equal_style_setter(info, klass, method_name)
      if /\Aset_/ =~ method_name and info.n_args == 1
        setter_method_name = "#{$POSTMATCH}="
        remove_existing_method(klass, setter_method_name)
        klass.__send__(:alias_method, setter_method_name, method_name)
      end
    end

    def load_function_infos(infos, klass)
      infos.each do |info|
        name = rubyish_method_name(info)
        next if name == "new"
        next if name == "alloc"
        define_singleton_method(klass, name, info)
      end
    end

    def load_interface_info(info)
      interface_module =
        self.class.define_interface(info.gtype, info.name, @base_module)
      load_methods(info, interface_module)
    end

    def load_constant_info(info)
      @base_module.const_set(info.name, info.value)
    end

    def load_union_info(info)
      klass = self.class.define_class(info.gtype, info.name, @base_module)
      load_fields(info, klass)
      load_methods(info, klass)
    end
  end
end
