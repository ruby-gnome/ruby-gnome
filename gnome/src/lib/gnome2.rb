require 'gnomecanvas2'
require 'gnome2.so'

module Gnome
  LOG_DOMAIN = "Gnome"
end

GLib::Log.set_log_domain(Gnome::LOG_DOMAIN)
