# Copyright (C) 2002-2015  Ruby-GNOME2 Project Team
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

require "glib2"

base_dir = Pathname.new(__FILE__).dirname.dirname.expand_path
vendor_dir = base_dir + "vendor" + "local"
vendor_bin_dir = vendor_dir + "bin"
GLib.prepend_dll_path(vendor_bin_dir)

if vendor_dir.exist?
  require "cairo"
end

begin
  major, minor, _ = RUBY_VERSION.split(/\./)
  require "#{major}.#{minor}/gdk_pixbuf2.so"
rescue LoadError
  require "gdk_pixbuf2.so"
end

if vendor_dir.exist?
  begin
    require "gobject-introspection"
    vendor_girepository_dir = vendor_dir + "lib" + "girepository-1.0"
    GObjectIntrospection.prepend_typelib_path(vendor_girepository_dir)
  rescue LoadError
  end
end

module Gdk
  class PixbufLoader
    def self.open(type = nil, mime_type = false)
      loader = Gdk::PixbufLoader.new(type, mime_type)
      if block_given?
        begin
          yield(loader)
        ensure
          loader.close
        end
      end
      loader
    end
  end

  class Pixbuf
    LOG_DOMAIN = "GdkPixbuf"

    class << self
      def cairo_available?
        if Gdk.respond_to?(:cairo_available?)
          Gdk.cairo_available?
        else
          false
        end
      end
    end

    module Version
      MAJOR = Gdk::Pixbuf::MAJOR
      MINOR = Gdk::Pixbuf::MINOR
      MICRO = Gdk::Pixbuf::MICRO
      STRING = "#{MAJOR}.#{MINOR}.#{MICRO}"

      class << self
        def or_later?(major, minor, micro=nil)
          micro ||= 0
          version = [
            MAJOR,
            MINOR,
            MICRO,
          ]
          (version <=> [major, minor, micro]) >= 0
        end
      end
    end
  end
end

GLib::Log.set_log_domain(Gdk::Pixbuf::LOG_DOMAIN)
