# Copyright (C) 2013-2019  Ruby-GNOME Project Team
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
    end

    def require_extension
      require "gio2.so"
    end

    def require_libraries
      require "gio2/action"
      require "gio2/action-map"
      require "gio2/application-command-line"
      require "gio2/content-type"
      require "gio2/file"
      require "gio2/icon"
      require "gio2/inet-address"
      require "gio2/input-stream"
      require "gio2/menu-item"
      require "gio2/pollable-input-stream"
      require "gio2/pollable-output-stream"
      require "gio2/resources"
      require "gio2/settings"
      require "gio2/settings-schema-source"
      require "gio2/simple-action"

      require "gio2/deprecated"
    end

    def define_content_type_class
      @content_type_class = Class.new do
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
      @dbus_module = Module.new
      @base_module.const_set("DBus", @dbus_module)
    end

    def define_resources_module
      @resources_module = Module.new
      @base_module.const_set("Resources", @resources_module)
    end

    def load_function_info(info)
      name = info.name
      case name
      when /\Acontent_type_/
        load_function_info_content_type(info)
      when "content_types_get_registered"
        define_singleton_method(@content_type_class, "registered", info)
      when /\Aresources_/
        name = rubyish_method_name(info, :prefix => "resources_")
        define_singleton_method(@resources_module, name, info)
      when /\Adbus_/
        name = rubyish_method_name(info, :prefix => "dbus_")
        define_singleton_method(@dbus_module, name, info)
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
        define_singleton_method(@content_type_class, name, info)
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

    def should_unlock_gvl?(function_info, klass)
      case klass.name
      when "Gio::InputStream"
        case function_info.name
        when "read", "read_all"
          true
        else
          false
        end
      else
        false
      end
    end
  end
end
