require "glib2"
require "gdk_pixbuf2"
begin
  require "gtk2"
rescue LoadError
end
begin
  require "cairo"
rescue LoadError
end
require "poppler.so"

module Poppler
  LOG_DOMAIN = "Poppler"

  VERSION = version.split(".").collect {|x| x.to_i}
end

if Poppler.cairo_available?
  module Cairo
    class Context
      def render_poppler_page(page, *args, &block)
        page.render(self, *args, &block)
      end

      def render_poppler_page_selection(page, *args, &block)
        page.render_selection(self, *args, &block)
      end
    end
  end
end

GLib::Log.set_log_domain(Poppler::LOG_DOMAIN)
