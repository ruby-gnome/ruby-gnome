require "glib2"
require "gdk_pixbuf2"
require "rsvg2.so"

module RSVG
  LOG_DOMAIN = "librsvg"

  if respond_to?(:init)
    init
    at_exit{term}
  end
end

GLib::Log.set_log_domain(RSVG::LOG_DOMAIN)
