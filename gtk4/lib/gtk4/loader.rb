# Copyright (C) 2014-2018  Ruby-GNOME2 Project Team
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

module Gtk
  class Loader < GObjectIntrospection::Loader
    def initialize(base_module, init_arguments)
      super(base_module)
      @init_arguments = init_arguments
    end

    def load
      self.version = "4.0"
      super("Gtk")
    end

    private

    def pre_load(repository, namespace)
      call_init_function(repository, namespace)
      define_version_module
    end

    def call_init_function(repository, namespace)
      init_check = repository.find(namespace, "init_check")
      arguments = [
        [$0] + @init_arguments,
      ]
      succeeded, argv = init_check.invoke(arguments)
      @init_arguments.replace(argv[1..-1]) unless argv.nil?
      raise InitError, "failed to initialize GTK+" unless succeeded
    end

    def define_version_module
      @version_module = Module.new
      @base_module.const_set("Version", @version_module)
    end

    def post_load(repository, namespace)
      require_extension
      require_libraries
    end

    def require_extension
    end

    def require_libraries
      require "gtk4/box"
      require "gtk4/builder"
      require "gtk4/button"
      require "gtk4/dialog"
      require "gtk4/menu-item"
      require "gtk4/message-dialog"
      require "gtk4/version"
      require "gtk4/window"
    end

    def load_constant_info(info)
      case info.name
      when /_VERSION\z/
        @version_module.const_set($PREMATCH, info.value)
      else
        super
      end
    end

    def rubyish_method_name(function_info, options={})
      name = function_info.name
      case name
      when /\Alist_(child_properties|style_properties)\z/
        $1
      else
        super
      end
    end
  end
end
