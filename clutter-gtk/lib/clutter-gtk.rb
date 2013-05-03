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

require "clutter"
require "gtk3"

base_dir = Pathname.new(__FILE__).dirname.dirname.expand_path
vendor_dir = base_dir + "vendor" + "local"
vendor_bin_dir = vendor_dir + "bin"
GLib.prepend_dll_path(vendor_bin_dir)
vendor_girepository_dir = vendor_dir + "lib" + "girepository-1.0"
GObjectIntrospection.prepend_typelib_path(vendor_girepository_dir)

module ClutterGtk
  LOG_DOMAIN = "Clutter-Gtk"
  GLib::Log.set_log_domain(LOG_DOMAIN)

  @initialized = false
  class << self
    def init(argv=ARGV)
      return if @initialized
      @initialized = true
      loader = Loader.new(self, argv)
      loader.load("GtkClutter")
      Clutter.init(argv)
    end
  end

  class Loader < GObjectIntrospection::Loader
    class InitError < StandardError
    end

    def initialize(base_module, init_arguments)
      super(base_module)
      @init_arguments = init_arguments
    end

    private
    def pre_load(repository, namespace)
      init = repository.find(namespace, "init")
      arguments = [
        1 + @init_arguments.size,
        [$0] + @init_arguments,
      ]
      error, argc, argv = init.invoke(arguments)
      @init_arguments.replace(argv)
      if error.to_i <= 0
        raise InitError, "failed to initialize Clutter: #{error.name}"
      end
    end

    def post_load(repository, namespace)
    end
  end
end
