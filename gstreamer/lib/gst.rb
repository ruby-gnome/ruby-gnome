# Copyright (C) 2013  Ruby-GNOME2 Project Team
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

  @initialized = false
  class << self
    def init(*argv)
      return if @initialized
      @initialized = true
      loader = Loader.new(self, argv)
      loader.load("Gst")
      require "gst/bin"
      require "gst/bus"
      require "gst/element"
      init_base
    end

    private
    def init_base
      require "gst/base_loader"
      base_loader = BaseLoader.new(self)
      base_loader.load("GstBase")
    end
  end

  class Loader < GObjectIntrospection::Loader
    def initialize(base_module, init_arguments)
      super(base_module)
      @init_arguments = init_arguments
    end

    private
    def pre_load(repository, namespace)
      init_check = repository.find(namespace, "init_check")
      arguments = [
        1 + @init_arguments.size,
        [$0] + @init_arguments,
      ]
      succeeded, argc, argv, error = init_check.invoke(arguments)
      @init_arguments.replace(argv[1..-1])
      raise error unless succeeded
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

    def load_function_info(info)
      return if info.name == "init"
      super
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
  end
end
