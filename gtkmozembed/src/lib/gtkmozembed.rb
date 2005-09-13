require 'gtk2'
require 'gtkmozembed.so'

module Gtk
  class MozEmbed
    LOG_DOMAIN = "GtkMozEmbed"
  end
end

GLib::Log.set_log_domain(Gtk::MozEmbed::LOG_DOMAIN)
