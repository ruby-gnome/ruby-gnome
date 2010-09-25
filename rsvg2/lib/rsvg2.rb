require "glib2"
require "gdk_pixbuf2"
begin
  require "cairo"
rescue LoadError
end

base_dir = Pathname.new(__FILE__).dirname.dirname.expand_path
vendor_dir = base_dir + "vendor" + "local"
vendor_bin_dir = vendor_dir + "bin"
GLib.prepend_environment_path(vendor_bin_dir)
begin
  major, minor, micro, = RUBY_VERSION.split(/\./)
  require "#{major}.#{minor}/rsvg2.so"
rescue LoadError
  require "rsvg2.so"
end

module RSVG
  LOG_DOMAIN = "librsvg"
end

if RSVG.cairo_available?
  module Cairo
    class Context
      def render_rsvg_handle(handle, *args, &block)
        handle.render_cairo(self, *args, &block)
      end
    end
  end
end

GLib::Log.set_log_domain(RSVG::LOG_DOMAIN)
