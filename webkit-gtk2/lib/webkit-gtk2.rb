# Copyright (C) 2013-2015  Ruby-GNOME2 Project Team
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
require "gtk2"

base_dir = Pathname.new(__FILE__).dirname.dirname.expand_path
vendor_dir = base_dir + "vendor" + "local"
vendor_bin_dir = vendor_dir + "bin"
GLib.prepend_dll_path(vendor_bin_dir)

module WebKitGtk2
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
      loader = Loader.new(self)
      loader.load
    end
  end

  class Loader < GObjectIntrospection::Loader
    NAMESPACE = "WebKit"
    VERSION = "1.0"

    def initialize(base_module)
      super
      @version = VERSION
    end

    def load
      super(NAMESPACE)
    end

    private
    def initialize_post(object)
      super
      return unless object.is_a?(GLib::Object)
      self.class.reference_gobject(object, :sink => true)
    end
  end
end
