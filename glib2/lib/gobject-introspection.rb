# Copyright (C) 2012-2019  Ruby-GNOME Project Team
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
      path = Pathname(path) unless path.is_a?(Pathname)
      return unless path.exist?

      dir = path.to_s
      dir = dir.gsub("/", File::ALT_SEPARATOR) if File::ALT_SEPARATOR
      return if Repository.search_path.include?(dir)

      Repository.prepend_search_path(dir)
    end
  end
end

require "gobject_introspection.so"

module GObjectIntrospection
  LOG_DOMAIN = "GObjectIntrospection"

  class << self
    def load(namespace, options={})
      base_module = Module.new
      loader = Loader.new(base_module)
      loader.version = options[:version]
      loader.load(namespace)
      base_module
    end
  end
end
GLib::Log.set_log_domain(GObjectIntrospection::LOG_DOMAIN)

require "gobject-introspection/arg-info"
require "gobject-introspection/boxed-info"
require "gobject-introspection/callable-info"
require "gobject-introspection/function-info"
require "gobject-introspection/interface-info"
require "gobject-introspection/object-info"
require "gobject-introspection/registered-type-info"
require "gobject-introspection/repository"
require "gobject-introspection/struct-info"
require "gobject-introspection/type-info"
require "gobject-introspection/type-tag"
require "gobject-introspection/union-info"

require "gobject-introspection/version"
require "gobject-introspection/loader"
