# Copyright (C) 2012-2013  Ruby-GNOME2 Project Team
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

require "glib2"

module GObjectIntrospection
  class << self
    def prepend_typelib_path(path)
      GLib.prepend_path_to_environment_variable(path, "GI_TYPELIB_PATH")
    end
  end
end

base_dir = Pathname.new(__FILE__).dirname.dirname.expand_path
vendor_dir = base_dir + "vendor" + "local"
vendor_bin_dir = vendor_dir + "bin"
GLib.prepend_dll_path(vendor_bin_dir)
begin
  major, minor, _ = RUBY_VERSION.split(/\./)
  require "#{major}.#{minor}/gobject_introspection.so"
rescue LoadError
  require "gobject_introspection.so"
end

vendor_girepository_dir = vendor_dir + "lib" + "girepository-1.0"
GObjectIntrospection.prepend_typelib_path(vendor_girepository_dir)

module GObjectIntrospection
  LOG_DOMAIN = "GObjectIntrospection"
end
GLib::Log.set_log_domain(GObjectIntrospection::LOG_DOMAIN)

require "gobject-introspection/repository"
require "gobject-introspection/callable-info"
require "gobject-introspection/object-info"
require "gobject-introspection/interface-info"
require "gobject-introspection/struct-info"
require "gobject-introspection/boxed-info"
require "gobject-introspection/union-info"
require "gobject-introspection/loader"
