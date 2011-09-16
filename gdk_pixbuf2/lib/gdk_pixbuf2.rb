require 'glib2'

base_dir = Pathname.new(__FILE__).dirname.dirname.expand_path
vendor_dir = base_dir + "vendor" + "local"
vendor_bin_dir = vendor_dir + "bin"
GLib.prepend_environment_path(vendor_bin_dir)
begin
  major, minor, _ = RUBY_VERSION.split(/\./)
  require "#{major}.#{minor}/gdk_pixbuf2.so"
rescue LoadError
  require "gdk_pixbuf2.so"
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
  end
end

GLib::Log.set_log_domain(Gdk::Pixbuf::LOG_DOMAIN)
