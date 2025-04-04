# Copyright (C) 2013-2018  Ruby-GNOME2 Project Team
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

require "gdk_pixbuf2"
require "clutter"
require "gst"

module ClutterGst
  LOG_DOMAIN = "Clutter-GStreamer"
  GLib::Log.set_log_domain(LOG_DOMAIN)

  class << self
    attr_accessor :load_version

    def const_missing(name)
      init
      if const_defined?(name)
        const_get(name)
      else
        super
      end
    end

    def init(argv=[])
      class << self
        remove_method(:init)
        remove_method(:const_missing)
      end
      Gst.init if Gst.respond_to?(:init)
      Clutter.init if Clutter.respond_to?(:init)

      loader = Loader.new(self, argv)
      loader.version = load_version
      loader.load
    end
  end

  self.load_version = nil

  class InitError < StandardError
  end

  class Loader < GObjectIntrospection::Loader
    def initialize(base_module, init_arguments)
      super(base_module)
      @init_arguments = init_arguments
    end

    def load
      super("ClutterGst")
    end

    private
    def pre_load(repository, namespace)
      init = repository.find(namespace, "init")
      arguments = [
        [$0] + @init_arguments,
      ]
      error, returned_arguments = init.invoke(arguments)
      @init_arguments.replace(returned_arguments[1..-1])
      if error.to_i <= 0
        raise InitError, "failed to initialize Clutter-GStreamer: #{error.name}"
      end
    end

    def post_load(repository, namespace)
      require "clutter-gst/version"
    end
  end
end
