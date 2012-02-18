=begin
  Copyright (c) 2006-2011 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
=end


require 'atk'
require 'gdk3'

base_dir = Pathname.new(__FILE__).dirname.dirname.dirname.expand_path
vendor_dir = base_dir + "vendor" + "local"
vendor_bin_dir = vendor_dir + "bin"
GLib.prepend_environment_path(vendor_bin_dir)
begin
  major, minor, _ = RUBY_VERSION.split(/\./)
  require "#{major}.#{minor}/gtk3.so"
rescue LoadError
  require "gtk3.so"
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

GLib::Log.set_log_domain(Gtk::LOG_DOMAIN)
