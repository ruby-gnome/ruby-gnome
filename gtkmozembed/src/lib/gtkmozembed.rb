require 'gtk2'
require 'gtkmozembed.so'

module Gtk
  class MozEmbed
    LOG_DOMAIN = "GtkMozEmbed"

    constants.each do |name|
      case name.to_s
      when /\AFAILED_USER([A-Z]+)\z/
        const_set("FAILED_USER_#{$1}", const_get(name))
      when /\ARELOADBYPASSPROXYAND([A-Z]+)\z/
        const_set("RELOAD_BYPASS_PROXY_AND_#{$1}", const_get(name))
      when /\ARELOADBYPASS([A-Z]+)\z/
        const_set("RELOAD_BYPASS_#{$1}", const_get(name))
      when /\ARELOADCHARSET([A-Z]+)\z/
        const_set("RELOAD_CHARSET_#{$1}", const_get(name))
      when /\A(WINDOW|EXTRA|PERSONAL)([A-Z]+)ON\z/
        const_set("#{$1}_#{$2}_ON", const_get(name))
      when /\A(RELOAD|DEFAULT|MENU|ALL|WINDOW|CENTER)([A-Z]+)\z/
        const_set("#{$1}_#{$2}", const_get(name))
      when /\AOPENAS([A-Z]+)\z/
        const_set("OPEN_AS_#{$1}", const_get(name))
      when /\A([A-Z]+)ON\z/
        const_set("#{$1}_ON", const_get(name))
      end
    end
  end
end

GLib::Log.set_log_domain(Gtk::MozEmbed::LOG_DOMAIN)
