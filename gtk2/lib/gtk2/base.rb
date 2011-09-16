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
require 'gdk_pixbuf2'

base_dir = Pathname.new(__FILE__).dirname.dirname.dirname.expand_path
vendor_dir = base_dir + "vendor" + "local"
vendor_bin_dir = vendor_dir + "bin"
GLib.prepend_environment_path(vendor_bin_dir)
begin
  major, minor, _ = RUBY_VERSION.split(/\./)
  require "#{major}.#{minor}/gtk2.so"
rescue LoadError
  require "gtk2.so"
end

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

  if check_version?(2, 12, 0)
    class Builder
      private
      def canonical_handler_name(name)
        name.gsub(/[-\s]+/, "_")
      end

      def __connect_signals__(connector, object, signal_name,
                              handler_name, connect_object, flags)
        handler_name = canonical_handler_name(handler_name)
        if connect_object
          handler = connect_object.method(handler_name)
        else
          handler = connector.call(handler_name)
        end
        unless handler
          $stderr.puts("Undefined handler: #{handler_name}") if $DEBUG
          return
        end

        if flags.after?
          signal_connect_method = :signal_connect_after
        else
          signal_connect_method = :signal_connect
        end

        if handler.arity.zero?
          object.send(signal_connect_method, signal_name) {handler.call}
        else
          object.send(signal_connect_method, signal_name, &handler)
        end
      end
    end
  end
end

GLib::Log.set_log_domain(Gdk::LOG_DOMAIN)
GLib::Log.set_log_domain(Gtk::LOG_DOMAIN)
