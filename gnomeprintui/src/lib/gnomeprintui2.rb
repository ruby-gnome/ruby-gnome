require "gtk2"
require "gnomeprint2"
require "gnomeprintui2.so"

module Gnome
  module PrintUI
    LOG_DOMAIN = "libgnomeprintui"
  end
end

GLib::Log.set_log_domain(Gnome::PrintUI::LOG_DOMAIN)
