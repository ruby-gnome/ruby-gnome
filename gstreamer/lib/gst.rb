# Copyright (C) 2013-2015  Ruby-GNOME2 Project Team
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

require "gobject-introspection"

base_dir = Pathname.new(__FILE__).dirname.dirname.expand_path
vendor_dir = base_dir + "vendor" + "local"
vendor_bin_dir = vendor_dir + "bin"
GLib.prepend_dll_path(vendor_bin_dir)
vendor_girepository_dir = vendor_dir + "lib" + "girepository-1.0"
GObjectIntrospection.prepend_typelib_path(vendor_girepository_dir)

if vendor_dir.exist?
  require "cairo"
  require "gdk_pixbuf2"
  require "pango"
end

module Gst
  LOG_DOMAIN = "GStreamer"
  GLib::Log.set_log_domain(LOG_DOMAIN)

  class << self
    def const_missing(name)
      init()
      if const_defined?(name)
        const_get(name)
      else
        super
      end
    end

    def method_missing(name, *args, &block)
      init()
      if respond_to?(name)
        __send__(name, *args, &block)
      else
        super
      end
    end

    def init(*argv)
      class << self
        remove_method(:init)
        remove_method(:const_missing)
        remove_method(:method_missing)
      end
      loader = Loader.new(self, argv)
      loader.load
      require "gst/bin"
      require "gst/bus"
      require "gst/caps"
      require "gst/element"
      require "gst/element-factory"
      require "gst/plugin-feature"
      require "gst/registry"
      require "gst/structure"
      init_base
      init_controller
    end

    private
    def init_base
      require "gst/base-loader"
      base_loader = BaseLoader.new(self)
      base_loader.load
    end

    def init_controller
      require "gst/controller-loader"
      controller_loader = ControllerLoader.new(self)
      controller_loader.load
    end
  end

  class Loader < GObjectIntrospection::Loader
    NAMESPACE = "Gst"

    def initialize(base_module, init_arguments)
      super(base_module)
      @init_arguments = init_arguments
    end

    def load
      super(NAMESPACE)
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
      succeeded, argv, error = init_check.invoke(:arguments => arguments)
      @init_arguments.replace(argv[1..-1])
      raise error unless succeeded
    end

    def define_value_modules
      @value_functions_module = Module.new
      @value_methods_module   = Module.new
      @base_module.const_set("ValueFunctions", @value_functions_module)
      @base_module.const_set("ValueMethods",   @value_methods_module)
    end

    def post_load(repository, namespace)
      require_extension
      self.class.start_callback_dispatch_thread
    end

    def require_extension
      begin
        major, minor, _ = RUBY_VERSION.split(/\./)
        require "#{major}.#{minor}/gstreamer.so"
      rescue LoadError
        require "gstreamer.so"
      end
    end

    def initialize_post(object)
      super
      return unless object.is_a?(GLib::Object)
      self.class.reference_gobject(object, :sink => true)
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
