require 'gtk2'
require 'gtkglext.so'
require 'opengl'

module Gdk
  module GL
    LOG_DOMAIN="GdkGLExt"
  end
end

module Gtk
  module GL
    LOG_DOMAIN="GtkGLExt"
  end
end

GLib::Log.set_log_domain(Gdk::GL::LOG_DOMAIN)
GLib::Log.set_log_domain(Gtk::GL::LOG_DOMAIN)
