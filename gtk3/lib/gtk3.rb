# Copyright (C) 2006-2014  Ruby-GNOME2 Project Team
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
require "atk"
require "gdk3"
require "gio2"

base_dir = Pathname.new(__FILE__).dirname.dirname.expand_path
vendor_dir = base_dir + "vendor" + "local"
vendor_bin_dir = vendor_dir + "bin"
GLib.prepend_dll_path(vendor_bin_dir)
vendor_girepository_dir = vendor_dir + "lib" + "girepository-1.0"
GObjectIntrospection.prepend_typelib_path(vendor_girepository_dir)

require "gtk3/loader"

module Gtk
  LOG_DOMAIN = "Gtk"
  GLib::Log.set_log_domain(LOG_DOMAIN)

  class Error < StandardError
  end

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
      Gdk.init if Gdk.respond_to?(:init)
      loader = Loader.new(self, argv)
      loader.load("Gtk")
    end
  end
end
