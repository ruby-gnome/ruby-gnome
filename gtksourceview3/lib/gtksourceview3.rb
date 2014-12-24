=begin
  Copyright (c) 2014 Ruby-GNOME2 Project Team
  This program is licensed under the same licence as Ruby-GNOME2.
=end

require 'gtk3'

base_dir = Pathname.new(__FILE__).dirname.dirname.expand_path
vendor_dir = base_dir + "vendor" + "local"
vendor_bin_dir = vendor_dir + "bin"
GLib.prepend_dll_path(vendor_bin_dir)
vendor_girepository_dir = vendor_dir + "lib" + "girepository-1.0"
GObjectIntrospection.prepend_typelib_path(vendor_girepository_dir)

require 'gtksourceview3/loader'

module GtkSource
  LOG_DOMAIN = "GtkSource"
  GLib::Log.set_log_domain(LOG_DOMAIN)

  class Error < StandardError
  end

  class << self
    def const_missing(name)
      init
      if const_defined?(name)
        const_get(name)
      else
        super
      end
    end

    def init
      class << self
        remove_method(:init)
        remove_method(:const_missing)
      end
      Gtk.init if Gtk.respond_to?(:init)
      loader = Loader.new(self)
      loader.load("GtkSource")
    end
  end
end
