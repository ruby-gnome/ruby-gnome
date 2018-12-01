# Copyright (C) 2013-2018  Ruby-GNOME2 Project Team
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

module WebKitGtk
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
      loader.load("WebKit")
    end
  end

  class Loader < GObjectIntrospection::Loader
    private
    def pre_load(repository, namespace)
      define_version_module
    end

    def post_load(repository, namespace)
      require_libraries
    end

    def define_version_module
      @version_module = Module.new
      @base_module.const_set("Version", @version_module)
    end

    def require_libraries
      require "webkit-gtk/version"
    end

    def load_constant_info(info)
      case info.name
      when /_VERSION\z/
        @version_module.const_set($PREMATCH, info.value)
      else
        super
      end
    end
  end
end
