require "glib2"
require "gdk_pixbuf2"
begin
  require "cairo"
rescue LoadError
end
require "rsvg2.so"

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
