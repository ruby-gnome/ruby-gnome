# Copyright (C) 2013-2014  Ruby-GNOME2 Project Team
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

require "gobject-introspection"
require "cairo-gobject"
require "atk"
require "pango"
require "gdk_pixbuf2"

require "gdk3/loader"
require "gdk3/x11-loader"

module Gdk
  LOG_DOMAIN = "Gdk"
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

    @@init_hooks = []
    def init
      class << self
        remove_method(:init)
        remove_method(:const_missing)
      end
      loader = Loader.new(self)
      loader.load
      x11_loader = X11Loader.new(self)
      x11_loader.load
      if Object.const_defined?(:Gtk) and Gtk.respond_to?(:init)
        Gtk.init
      end

      @@init_hooks.each do |hook|
        hook.call
      end
    end

    def on_init(&block)
      @@init_hooks << block
    end

    def cairo_available?
      true
    end
  end
end
