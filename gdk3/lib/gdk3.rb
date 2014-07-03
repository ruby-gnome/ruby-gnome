# Copyright (C) 2013-2014  Ruby-GNOME2 Project Team
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
require "cairo-gobject"
require "atk"
require "pango"
require "gdk_pixbuf2"

base_dir = Pathname.new(__FILE__).dirname.dirname.expand_path
vendor_dir = base_dir + "vendor" + "local"
vendor_bin_dir = vendor_dir + "bin"
GLib.prepend_dll_path(vendor_bin_dir)
vendor_girepository_dir = vendor_dir + "lib" + "girepository-1.0"
GObjectIntrospection.prepend_typelib_path(vendor_girepository_dir)

require "gdk3/loader"

module Gdk
  LOG_DOMAIN = "Gdk"
  GLib::Log.set_log_domain(LOG_DOMAIN)

  class Error < StandardError
  end

  class << self
    def const_missing(name)
      init
      if const_defined?(name)
        const_get(name)
      else
        super
      end
    end

    def init
      class << self
        remove_method(:init)
        remove_method(:const_missing)
      end
      loader = Loader.new(self)
      loader.load("Gdk")
    end

    def cairo_available?
      true
    end
  end
end

if Gdk.cairo_available?
  module Cairo
    class Context
      def set_source_gdk_color(color)
        Gdk.cairo_set_source_color(self, color)
      end

      if method_defined?(:set_source_color)
        alias_method :set_source_not_gdk_color, :set_source_color
        def set_source_color(color)
          if color.is_a?(Gdk::Color)
            set_source_gdk_color(color)
          else
            set_source_not_gdk_color(color)
          end
        end
      else
        alias_method :set_source_color, :set_source_gdk_color
      end

      def source_color=(color)
        set_source_color(color)
        color
      end

      def gdk_rectangle(rectangle)
        Gdk.cairo_rectangle(self, rectangle)
      end

      def set_source_pixbuf(pixbuf, pixbuf_x=0, pixbuf_y=0)
        Gdk.cairo_set_source_pixbuf(self, pixbuf, pixbuf_x, pixbuf_y)
      end
    end
  end
end
