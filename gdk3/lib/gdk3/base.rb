=begin
  Copyright (c) 2006-2011 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
=end

require 'glib2'
require 'pango'
require 'gdk_pixbuf2'

base_dir = Pathname.new(__FILE__).dirname.dirname.dirname.expand_path
vendor_dir = base_dir + "vendor" + "local"
vendor_bin_dir = vendor_dir + "bin"
GLib.prepend_dll_path(vendor_bin_dir)
begin
  major, minor, _ = RUBY_VERSION.split(/\./)
  require "#{major}.#{minor}/gdk3.so"
rescue LoadError
  require "gdk3.so"
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
  LOG_DOMAIN = "Gdk"
end

if Gdk.cairo_available?
  module Cairo
    class Context
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
    end
  end
end

GLib::Log.set_log_domain(Gdk::LOG_DOMAIN)

