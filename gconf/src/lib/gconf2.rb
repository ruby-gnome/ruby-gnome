require 'glib2'
require 'gconf2.so'

module GConf
  LOG_DOMAIN = "GConf"
end

GLib::Log.set_log_domain(GConf::LOG_DOMAIN)
