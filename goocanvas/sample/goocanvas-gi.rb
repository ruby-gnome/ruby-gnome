# Copyright (C) 2013  Ruby-GNOME2 Project Team
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
require "gobject-introspection"

base_dir = Pathname.new(__FILE__).dirname.dirname.dirname.expand_path
vendor_dir = base_dir + "vendor" + "local"
vendor_bin_dir = vendor_dir + "bin"
GLib.prepend_dll_path(vendor_bin_dir)

if vendor_dir.exist?
  begin
    require "gobject-introspection"
    vendor_girepository_dir = vendor_dir + "lib" + "girepository-1.0"
    GObjectIntrospection.prepend_typelib_path(vendor_girepository_dir)
  rescue LoadError
  end
end

module Goo
  LOG_DOMAIN = "GooCanvas"
  GLib::Log.set_log_domain(LOG_DOMAIN)

  @initialized = false
  class << self
    def init
      return if @initialized
      @initialized = true
      loader = Loader.new(self)
      loader.load("GooCanvas")
      require "goocanvas/canvas-item"
    end
  end

  class Loader < GObjectIntrospection::Loader
    private
    def load_field(info, i, field_info, klass)
      return if field_info.name == "parent"
      super
    end
  end
end
