# Copyright (C) 2012-2019  Ruby-GNOME2 Project Team
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
        load_info(info) if info.is_a?(InterfaceInfo)
      end
      repository.each(namespace) do |info|
        load_info(info) unless info.is_a?(InterfaceInfo)
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
      function_info.unlock_gvl = should_unlock_gvl?(function_info, target_module)
      full_method_name = "#{target_module}\#.#{name}"
      invoker = Invoker.new(function_info, name, full_method_name)
      target_module.module_eval do
        define_method(name) do |*arguments, &block|
          invoker.invoke(nil, arguments, block)
        end
        module_function(name)
      end
    end

    def define_singleton_method(klass, name, info)
      info.unlock_gvl = should_unlock_gvl?(info, klass)
      invoker = Invoker.new(info, name, "#{klass}.#{name}")
      klass.singleton_class.__send__(:define_method, name) do |*arguments, &block|
        invoker.invoke(nil, arguments, block)
      end
    end

    def define_struct(info, options={})
      name = rubyish_class_name(info)
      if info.gtype == GLib::Type::NONE
        klass = self.class.define_struct(info.size, name, @base_module,
                                         :parent => options[:parent])
      else
        size = info.size
        size = nil if size.zero?
        klass = self.class.define_class(info.gtype, name, @base_module,
                                        :parent => options[:parent],
                                        :size   => size)
      end
      load_fields(info, klass)
      load_methods(info, klass)
    end

    def load_struct_info(info)
      case info.name
      when /Class\z/
        base_class_name = rubyish_class_name(info)
        method_infos = info.methods.find_all do |method_info|
          method_info.is_a?(MethodInfo)
        end
        unless method_infos.empty?
          base_class = @base_module.const_get(base_class_name)
          load_method_infos(method_infos, base_class.singleton_class)
        end
      else
        return if info.gtype_struct?

        define_struct(info)
      end
    end

    def define_boxed(info)
      return if info.gtype == GLib::Type::NONE
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
      # TODO: Can we do the same things for flags on NONE GType?
      return if info.gtype == GLib::Type::NONE
      self.class.define_class(info.gtype,
                              rubyish_class_name(info),
                              @base_module)
    end

    def define_error(info)
      self.class.define_error(info.error_domain,
                              rubyish_class_name(info),
                              @base_module,
                              :parent => error_parent_class(info),
                              :gtype => info.gtype)
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
        @base_module.const_set(rubyish_class_name(info), enum_module)
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
        @base_module.const_set(rubyish_class_name(info), flags_module)
      else
        self.class.define_class(info.gtype, flags_class_name(info), @base_module)
      end
    end

    def flags_class_name(info)
      info.name
    end

    def load_object_info(info)
      return if info.gtype == GLib::Type::NONE
      klass = self.class.define_class(info.gtype,
                                      rubyish_class_name(info),
                                      @base_module)
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

    def load_field(info, i, field_info, klass, options={})
      name = field_name(field_info, klass)
      flags = field_info.flags

      readable = options[:readable]
      readable = flags.readable? if readable.nil?
      if readable
        reader_method_name = rubyish_field_reader_name(field_info, name)
        load_field_reader(info, i, field_info, klass, name, reader_method_name)
      end

      writable = options[:writable]
      writable = flags.writable? if writable.nil?
      if writable
        load_field_writer(info, i, field_info, klass, name, "#{name}=")
      end
    end

    def load_field_reader(info, i, field_info, klass, name, method_name)
      remove_existing_method(klass, method_name)

      need_number_to_bool_convert = false
      if name.start_with?("is_") and
          field_info.type.tag == TypeTag::UINT32
        need_number_to_bool_convert = true
      end
      klass.__send__(:define_method, method_name) do ||
        value = info.get_field_value(self, i)
        if need_number_to_bool_convert
          value != 0
        else
          value
        end
      end
    end

    def load_field_writer(info, i, field_info, klass, name, method_name)
      klass.__send__(:define_method, method_name) do |value|
        info.set_field_value(self, i, value)
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
      if klass.method_defined?(:each) and
          klass.instance_method(:each).owner == klass
        klass.include(Enumerable)
      end
    end

    def load_constructor_infos(infos, klass)
      return if infos.empty?

      call_initialize_post = lambda do |object|
        initialize_post(object)
      end
      infos.each do |info|
        name = "initialize_#{info.name}"
        info.unlock_gvl = should_unlock_gvl?(info, klass)
        invoker = Invoker.new(info, name, "#{klass}\##{name}")
        klass.__send__(:define_method, name) do |*arguments, &block|
          invoker.invoke(self, arguments, block)
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
      return true if argument.nil? and arg_info.may_be_null?

      arg_info.type.match?(argument)
    end

    def rubyish_method_name(function_info, options={})
      name = function_info.name
      if options[:prefix]
        name = name.gsub(/\A#{Regexp.escape(options[:prefix])}/, "")
      end
      if name == "initialize"
        name += "_raw"
      end

      n_in_args = function_info.n_in_args
      if options[:n_in_args_offset]
        n_in_args += options[:n_in_args_offset]
      end

      return_type = function_info.return_type
      return_type_tag = return_type.tag
      if return_type_tag == TypeTag::VOID
        out_arg_tags = function_info.out_args.collect {|arg| arg.type.tag}
        if out_arg_tags == [TypeTag::ARRAY, TypeTag::INT32]
          return_type_tag = TypeTag::ARRAY
        end
      end

      case return_type_tag
      when TypeTag::BOOLEAN
        case name
        when "equal"
          if n_in_args == 1
            "=="
          else
            name
          end
        when "not_equal"
          if n_in_args == 1
            "!="
          else
            name
          end
        when "less_than"
          if n_in_args == 1
            "<"
          else
            name
          end
        when "less_than_or_equal"
          if n_in_args == 1
            "<="
          else
            name
          end
        when "greater_than"
          if n_in_args == 1
            ">"
          else
            name
          end
        when "greater_than_or_equal"
          if n_in_args == 1
            ">="
          else
            name
          end
        when /\A(?:is|get_is|can_be)_/
          "#{$POSTMATCH}?"
        when /\Aget_/
          if n_in_args.zero?
            if function_info.n_out_args.zero?
              "#{$POSTMATCH}?"
            else
              $POSTMATCH
            end
          else
            name
          end
        when /\A(?:has|use|can|in|on|.*_is)_/
          "#{name}?"
        when "exists"
          "exist?"
        else
          name
        end
      when TypeTag::GLIST, TypeTag::GSLIST, TypeTag::ARRAY
        case name
        when /\A(?:list|get)_/
          if n_in_args.zero?
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
          if n_in_args.zero?
            $POSTMATCH
          else
            name
          end
        when "to_string"
          "to_s"
        when "to_integer"
          "to_i"
        when "foreach"
          "each"
        else
          name
        end
      end
    end

    def rubyish_field_reader_name(field_info, name)
      case field_info.type.tag
      when TypeTag::BOOLEAN
        name.gsub(/\Ais_/, "") + "?"
      when TypeTag::UINT32
        if /\Ais_/ =~ name
          "#{$POSTMATCH}?"
        else
          name
        end
      else
        name
      end
    end

    def rubyish_class_name(info)
      name = info.name
      case info
      when StructInfo
        case name
        when /Class\z/
          $PREMATCH
        else
          name
        end
      else
        name
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
      define_inspect(info, klass, method_name)
    end

    def remove_existing_method(klass, method_name)
      return unless klass.method_defined?(method_name)
      return unless klass.instance_method(method_name).owner == klass
      klass.__send__(:remove_method, method_name)
    end

    def define_method(info, klass, method_name)
      info.unlock_gvl = should_unlock_gvl?(info, klass)
      remove_existing_method(klass, method_name)
      invoker = Invoker.new(info, method_name, "#{klass}\##{method_name}")
      klass.__send__(:define_method, method_name) do |*arguments, &block|
        invoker.invoke(self, arguments, block)
      end
    end

    def define_equal_style_setter(info, klass, method_name)
      if /\Aset_/ =~ method_name and info.n_in_args == 1
        setter_method_name = "#{$POSTMATCH}="
        remove_existing_method(klass, setter_method_name)
        klass.__send__(:alias_method, setter_method_name, method_name)
      end
    end

    def define_inspect(info, klass, method_name)
      if method_name == "to_s" and info.n_args.zero?
        klass.__send__(:define_method, "inspect") do ||
          super().gsub(/\>\z/) {" #{to_s}>"}
        end
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
      return if info.gtype == GLib::Type::NONE
      interface_module =
        self.class.define_interface(info.gtype,
                                    rubyish_class_name(info),
                                    @base_module)
      load_methods(info, interface_module)
    end

    def load_constant_info(info)
      define_constant(info.name, info)
    end

    def define_constant(name, info)
      @base_module.const_set(name, info.value)
    end

    def load_union_info(info)
      return if info.gtype == GLib::Type::NONE
      klass = self.class.define_class(info.gtype, info.name, @base_module)
      load_fields(info, klass)
      load_methods(info, klass)
    end

    class Invoker
      def initialize(info, method_name, full_method_name)
        @info = info
        @method_name = method_name
        @full_method_name = full_method_name
        @prepared = false
      end

      def invoke(receiver, arguments, block)
        ensure_prepared

        if receiver and @function_info_p
          arguments.unshift(receiver)
        end

        arguments, block = build(receiver, arguments, block)
        unless valid?(arguments)
          if @on_invalid == :fallback
            return @value_on_invalid
          else
            raise ArgumentError, invalid_error_message(arguments)
          end
        end

        if block.nil? and @require_callback_p
          receiver.to_enum(@method_name, *arguments)
        else
          if @function_info_p
            return_value = @info.invoke(arguments, &block)
          else
            return_value = @info.invoke(receiver, arguments, &block)
          end
          if @have_return_value_p
            return_value
          else
            receiver
          end
        end
      end

      private
      def ensure_prepared
        return if @prepared

        @in_args = @info.in_args
        @n_in_args = @in_args.size
        @n_required_in_args = @info.n_required_in_args
        @last_in_arg = @in_args.last
        if @last_in_arg
          @last_in_arg_is_gclosure = @last_in_arg.gclosure?
        else
          @last_in_arg_is_gclosure = false
        end
        @valid_n_args_range = (@n_required_in_args..@n_in_args)

        @in_arg_types = []
        @in_arg_nil_indexes = []
        @in_args.each_with_index do |arg, i|
          @in_arg_types << arg.type
          @in_arg_nil_indexes << i if arg.may_be_null?
        end

        @function_info_p = (@info.class == FunctionInfo)
        @have_return_value_p = @info.have_return_value?
        @require_callback_p = @info.require_callback?

        prepare_on_invalid

        @prepared = true
      end

      def prepare_on_invalid
        case @method_name
        when "=="
          @value_on_invalid = false
          @on_invalid = :fallback
        when "!="
          @value_on_invalid = true
          @on_invalid = :fallback
        else
          @on_invalid = :raise
        end
      end

      def build(receiver, arguments, block)
        if block and @last_in_arg_is_gclosure
          arguments << block
          block = nil
        end

        n_missing_arguments = @n_in_args - arguments.size
        if 0 < n_missing_arguments
          @in_arg_nil_indexes.each_with_index do |nil_index, i|
            next if i <= n_missing_arguments
            arguments.insert(nil_index, nil)
          end
        end

        return arguments, block
      end

      def valid?(arguments)
        return false unless @valid_n_args_range.cover?(arguments.size)
        if @on_invalid == :fallback
          arguments.zip(@in_arg_types) do |argument, type|
            return false unless type.match?(argument)
          end
        end
        true
      end

      def invalid_error_message(arguments)
        detail = "#{arguments.size} for "
        if @n_in_args == @n_required_in_args
          detail << "#{@n_in_args}"
        else
          detail << "#{@n_required_in_args}..#{@n_in_args}"
        end
        "#{@full_method_name}: wrong number of arguments (#{detail})"
      end
    end
  end
end
