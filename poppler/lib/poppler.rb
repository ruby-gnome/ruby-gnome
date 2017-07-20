# Copyright (C) 2017  Ruby-GNOME2 Project Team
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
require "gio2"

base_dir = Pathname.new(__FILE__).dirname.dirname.expand_path
vendor_dir = base_dir + "vendor" + "local"
vendor_bin_dir = vendor_dir + "bin"
GLib.prepend_dll_path(vendor_bin_dir)
vendor_girepository_dir = vendor_dir + "lib" + "girepository-1.0"
GObjectIntrospection.prepend_typelib_path(vendor_girepository_dir)

require "poppler/loader"

module Poppler
  LOG_DOMAIN = "Poppler"
  GLib::Log.set_log_domain(LOG_DOMAIN)

  loader = Loader.new(self)
  loader.load("Poppler")

  module Version
    MAJOR = MAJOR_VERSION
    MINOR = MINOR_VERSION
    MICRO = MICRO_VERSION
    STRING = "#{MAJOR_VERSION}.#{MINOR_VERSION}.#{MICRO_VERSION}"
    class << self
      def or_later?(major, minor, micro=nil)
        micro ||= 0
        version = [
          MAJOR_VERSION,
          MINOR_VERSION,
          MICRO_VERSION,
        ]
        (version <=> [major, minor, micro]) >= 0
      end
    end
  end

  class AnnotCalloutLine
    alias_method :initialize_raw, :initialize
    def initialize(*args)
      initialize_raw
      if args.size == 1 && args.class == Hash
        self.multiline = args[:multiline] || false
        self.x1 = args[:x1] || 0
        self.y1 = args[:y1] || 0
        self.x2 = args[:x2] || 0
        self.y2 = args[:y2] || 0
        self.x3 = args[:x3] || 0
        self.y3 = args[:y3] || 0
      elsif args.class == Array && args.size == 7
        self.multiline = args[0]
        self.x1 = args[1]
        self.y1 = args[2]
        self.x2 = args[3]
        self.y2 = args[4]
        self.x3 = args[5]
        self.y3 = args[6]
      end
    end

    def to_a
      [multiline?, x1, y1, x2, y2, x3, y3]
    end
  end
end
