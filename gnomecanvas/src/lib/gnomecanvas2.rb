require 'gtk2'
require 'libart2'
require 'gnomecanvas2.so'

module Gnome
  class Canvas
    LOG_DOMAIN = "GnomeCanvas"
  end
end

GLib::Log.set_log_domain(Gnome::Canvas::LOG_DOMAIN)
