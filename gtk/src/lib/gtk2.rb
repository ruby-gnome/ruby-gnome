require 'glib2'
require 'atk'
require 'pango'
begin
  require 'cairo'
rescue LoadError
end
require 'gtk2.so'
require 'gdk_pixbuf2'

module Gdk
  LOG_DOMAIN = "Gdk"
end

module Gtk
  LOG_DOMAIN = "Gtk"
end

GLib::Log.set_log_domain(Gdk::LOG_DOMAIN)
GLib::Log.set_log_domain(Gtk::LOG_DOMAIN)
