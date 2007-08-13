#! /usr/bin/env ruby
=begin
  gtk2/base.rb

  Copyright (c) 2006 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: base.rb,v 1.6 2007/08/13 11:09:22 ktou Exp $
=end


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

if Gdk.cairo_available?
  module Cairo
    class Context
      if method_defined?(:set_source_color)
        alias_method :set_source_not_gdk_color, :set_source_color
        def set_source_color(color)
          if color.is_a?(Gdk::Color)
            set_source_gdk_color(color)
          else
            set_source_not_gdk_color(color)
          end
        end
      else
        alias_method :set_source_color, :set_source_gdk_color
      end

      def source_color=(color)
        set_source_color(color)
        color
      end
    end
  end
end


module Gtk
  LOG_DOMAIN = "Gtk"

  class Printer
    def self.printers(wait = false)
      printers = []
      self.each(wait) {|v| printers << v}
      printers
    end
  end
end

GLib::Log.set_log_domain(Gdk::LOG_DOMAIN)
GLib::Log.set_log_domain(Gtk::LOG_DOMAIN)
