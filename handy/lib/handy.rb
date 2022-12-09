# Copyright (C) 2022  Ruby-GNOME Project Team
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

require "gtk3"

require "handy/loader"

module Handy
  LOG_DOMAIN = "Handy"
  GLib::Log.set_log_domain(LOG_DOMAIN)

  class << self
    def const_missing(name)
      init()
      if const_defined?(name)
        const_get(name)
      else
        super
      end
    end

    def method_missing(name, *args, &block)
      init()
      if respond_to?(name)
        __send__(name, *args, &block)
      else
        super
      end
    end

    def init
      class << self
        remove_method(:init)
        remove_method(:const_missing)
        remove_method(:method_missing)
      end
      Gtk.init if Gtk.respond_to?(:init)
      loader = Loader.new(self)
      loader.load("Handy")
    end
  end
end
