require "glib2"
require "libart2"
require "pango"
require "gnomeprint2.so"

# for Rabbit <= 0.0.5
module Gnome
  class GPANode
    alias description value 
    alias name id
  end
  PrinterGPA = GPANode
end

module Gnome
  module Print
    LOG_DOMAIN = "GnomePrint"
  end
  
  module PrintCupsPlugin
    LOG_DOMAIN = "GnomePrintCupsPlugin"
  end
  
  module PrintPAPIPlugin
    LOG_DOMAIN = "GnomePrintPAPIPlugin"
  end
  
  module PrintLpdPlugin
    LOG_DOMAIN = "GnomePrintLpdPlugin"
  end
end

GLib::Log.set_log_domain(Gnome::Print::LOG_DOMAIN)
GLib::Log.set_log_domain(Gnome::PrintCupsPlugin::LOG_DOMAIN)
GLib::Log.set_log_domain(Gnome::PrintPAPIPlugin::LOG_DOMAIN)
GLib::Log.set_log_domain(Gnome::PrintLpdPlugin::LOG_DOMAIN)
