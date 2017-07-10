# Copyright (C) 2015-2017  Ruby-GNOME2 Project Team
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

module WebKit2Gtk
  class Loader < GObjectIntrospection::Loader
    NAMESPACE = "WebKit2"

    def load
      super(NAMESPACE)
    end

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
      require "webkit2-gtk/version" if @version_module.const_defined?(:MAJOR)

      require "webkit2-gtk/web-context"
      require "webkit2-gtk/web-view"
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
