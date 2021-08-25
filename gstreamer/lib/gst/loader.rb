# Copyright (C) 2013-2021  Ruby-GNOME Project Team
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

module Gst
  class Loader < GObjectIntrospection::Loader
    def initialize(base_module, init_arguments)
      super(base_module)
      @init_arguments = init_arguments
    end

    def load
      super("Gst")
    end

    private
    def pre_load(repository, namespace)
      call_init_function(repository, namespace)
      define_value_modules
    end

    def call_init_function(repository, namespace)
      init_check = repository.find(namespace, "init_check")
      arguments = [
        [$0] + @init_arguments,
      ]
      succeeded, argv, error = init_check.invoke(arguments)
      @init_arguments.replace(argv[1..-1])
      raise error unless succeeded
    end

    def define_value_modules
      @value_functions_module = define_methods_module(:ValueFunctions)
      @value_methods_module   = define_methods_module(:ValueMethods)
    end

    def post_load(repository, namespace)
      post_methods_module(@value_functions_module)
      post_methods_module(@value_methods_module)
      require_extension
      require_libraries
      self.class.start_callback_dispatch_thread
    end

    def require_extension
      require "gstreamer.so"
    end

    def require_libraries
      require "gst/bin"
      require "gst/bus"
      require "gst/caps"
      require "gst/element"
      require "gst/element-factory"
      require "gst/plugin-feature"
      require "gst/registry"
      require "gst/structure"
      require "gst/tag-list"
      require "gst/type-find-factory"
      require "gst/version"
    end

    def load_function_info(info)
      case info.name
      when "init"
        # ignore
      when /\Avalue_/
        method_name = $POSTMATCH
        load_value_function_info(info, method_name)
      else
        super
      end
    end

    def load_value_function_info(info, method_name)
      value_functions_module = @value_functions_module
      define_module_function(value_functions_module, method_name, info)
      @value_methods_module.module_eval do
        define_method(method_name) do |*arguments, &block|
          value_functions_module.send(method_name, self, *arguments, &block)
        end
      end
    end

    def load_method_info(info, klass, method_name)
      case method_name
      when "ref", "unref"
        # Ignore
      else
        super
      end
    end

    RENAME_MAP = {
      "uri_protocol_is_valid"     => "valid_uri_protocol?",
      "uri_protocol_is_supported" => "supported_uri_protocol?",
      "uri_is_valid"              => "valid_uri?",
      "uri_has_protocol"          => "uri_has_protocol?",
    }
    def rubyish_method_name(function_info)
      RENAME_MAP[function_info.name] || super
    end

    UNLOCK_GVL_METHODS = {
      "Gst::Element#set_state"   => true,
      "Gst::Element#get_state"   => true,
      "Gst::Element#query_state" => true,
      "Gst::Element#send_event"  => true,
    }
    def should_unlock_gvl?(function_info, klass)
      UNLOCK_GVL_METHODS["#{klass}\##{function_info.name}"] || super
    end
  end
end
