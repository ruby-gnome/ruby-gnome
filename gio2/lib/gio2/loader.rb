# Copyright (C) 2013-2025  Ruby-GNOME Project Team
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
  class Loader < GObjectIntrospection::Loader
    private
    def pre_load(repository, namespace)
      define_content_type_class
      define_mime_type_class
      define_dbus_module
      define_resources_module
      @file_function_infos = []
      @content_type_guess_for_tree_info = nil
    end

    def post_load(repository, namespace)
      if @content_type_guess_for_tree_info
        content_type_class = @content_type_class
        info = @content_type_guess_for_tree_info
        file_module = @base_module.const_get("File")
        file_module.__send__(:define_method, "guess_content_types") do
          info.invoke([self]).collect do |type|
            content_type_class.new(type)
          end
        end
      end
      require_extension
      require_libraries

      if defined?(Ractor)
        [
          @content_type_class,
          @mime_type_class,
          @dbus_module,
          @resources_module,
        ].each do |klass|
          Ractor.make_shareable(klass::INVOKERS)
          Ractor.make_shareable(klass.singleton_class::INVOKERS)
        end
      end
    end

    def require_extension
      require "gio2.so"
    end

    def require_libraries
      require_relative "action"
      require_relative "action-map"
      require_relative "application-command-line"
      require_relative "bytes-icon"
      require_relative "content-type"
      require_relative "file"
      require_relative "icon"
      require_relative "inet-address"
      require_relative "input-stream"
      require_relative "list-model"
      require_relative "menu-item"
      require_relative "output-stream"
      require_relative "pollable-input-stream"
      require_relative "pollable-output-stream"
      require_relative "resources"
      require_relative "ruby-input-stream"
      require_relative "ruby-output-stream"
      require_relative "settings"
      require_relative "settings-schema-source"
      require_relative "simple-action"
      require_relative "volume"

      require_relative "deprecated"
    end

    def define_content_type_class
      @content_type_class = Class.new do
        const_set(:INVOKERS, {})
        singleton_class.const_set(:INVOKERS, {})

        def initialize(type)
          @type = type
        end

        def to_s
          @type
        end
      end
      @base_module.const_set("ContentType", @content_type_class)
    end

    def define_mime_type_class
      @mime_type_class = Class.new do
        const_set(:INVOKERS, {})
        singleton_class.const_set(:INVOKERS, {})

        def initialize(type)
          @type = type
        end

        def to_s
          @type
        end
      end
      @base_module.const_set("MimeType", @mime_type_class)
    end

    def define_dbus_module
      @dbus_module = Module.new do
        const_set(:INVOKERS, {})
        singleton_class.const_set(:INVOKERS, {})
      end
      @base_module.const_set("DBus", @dbus_module)
    end

    def define_resources_module
      @resources_module = Module.new do
        const_set(:INVOKERS, {})
        singleton_class.const_set(:INVOKERS, {})
      end
      @base_module.const_set("Resources", @resources_module)
    end

    def load_function_info(info)
      name = info.name
      case name
      when /\Acontent_type_/
        load_function_info_content_type(info)
      when "content_types_get_registered"
        define_singleton_method(info, @content_type_class, "registered")
      when /\Aresources_/
        name = rubyish_method_name(info, :prefix => "resources_")
        define_singleton_method(info, @resources_module, name)
      when /\Adbus_/
        name = rubyish_method_name(info, :prefix => "dbus_")
        define_singleton_method(info, @dbus_module, name)
      when /\A{file,icon}_/
        # Ignore because they are defined by load_interface_info
      else
        super
      end
    end

    def load_function_info_content_type(info)
      name = rubyish_method_name(info, :prefix => "content_type_")
      case name
      when "equals", "is_a"
        case name
        when "equals"
          method_name = "=="
        when "is_a"
          method_name = "#{name}?"
        end
        @content_type_class.__send__(:define_method, method_name) do |other|
          if other.is_a?(self.class)
            other = other.to_s
          end
          if other.is_a?(String)
            info.invoke([to_s, other])
          else
            false
          end
        end
      when "from_mime_type"
        @mime_type_class.__send__(:define_method, "content_type") do
          info.invoke([to_s])
        end
      when "get_mime_type"
        mime_type_class = @mime_type_class
        @content_type_class.__send__(:define_method, "mime_type") do
          mime_type = info.invoke([to_s])
          if mime_type
            mime_type_class.new(mime_type)
          else
            nil
          end
        end
      when "guess_for_tree"
        @content_type_guess_for_tree_info = info
      when "guess"
        define_singleton_method(info, @content_type_class, name)
      else
        case name
        when /\Aget_/
          method_name = $POSTMATCH
        else
          method_name = name
        end
        @content_type_class.__send__(:define_method, method_name) do
          info.invoke([to_s])
        end
      end
    end

    def load_function_info_singleton_method(info, klass, method_name)
      case "#{klass.name}##{method_name}"
      when "Gio::Icon#hash"
        define_method(info, klass, method_name)
      when "Gio::Icon#new_for_string"
        super(info, klass, "find")
      else
        super
      end
    end

    def load_interface_info(info)
      super
      case info.gtype.name
      when "GListModel"
        # This is workaround for Ruby < 3.
        # AlmaLinux 8 and Ubuntu 20.04 still use Ruby < 3.
        require_relative "list-model"
      end
    end

    def rubyish_method_name(function_info, options={})
      case function_info.name
      when "get_mount"
        case function_info.container&.name
        when "Volume"
          "get_mount"
        else
          super
        end
      else
        super
      end
    end

    def rubyish_class_name(info)
      case info.name
      when /Enum\z/
        $PREMATCH
      else
        super
      end
    end

    def error_parent_class(info)
      Error
    end

    def prepare_function_info_lock_gvl(function_info, klass)
      super
      case klass.name
      when "Gio::Seekable"
        function_info.lock_gvl_default = false
        if defined?(Ractor)
          predicate = Ractor.current.instance_eval do
            lambda do |_, receiver|
              receiver.is_a?(RubySeekable)
            end
          end
          Ractor.make_shareable(predicate)
        else
          predicate = lambda do |_, receiver|
            receiver.is_a?(RubySeekable)
          end
        end
        function_info.add_lock_gvl_predicate(&predicate)
      when "Gio::InputStream"
        case function_info.name
        when "read", "read_all", "skip", "close"
          function_info.lock_gvl_default = false
          if defined?(Ractor)
            predicate = Ractor.current.instance_eval do
              lambda do |_, receiver|
                receiver.is_a?(RubyInputStream)
              end
            end
            Ractor.make_shareable(predicate)
          else
            predicate = lambda do |_, receiver|
              receiver.is_a?(RubyInputStream)
            end
          end
          function_info.add_lock_gvl_predicate(&predicate)
        end
      when "Gio::OutputStream"
        case function_info.name
        when "write", "write_all", "flush", "close"
          function_info.lock_gvl_default = false
          if defined?(Ractor)
            predicate = Ractor.current.instance_eval do
              lambda do |_, receiver|
                receiver.is_a?(RubyOutputStream)
              end
            end
            Ractor.make_shareable(predicate)
          else
            predicate = lambda do |_, receiver|
              receiver.is_a?(RubyOutputStream)
            end
          end
          function_info.add_lock_gvl_predicate(&predicate)
        end
      end
    end
  end
end
