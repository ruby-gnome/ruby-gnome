# Copyright (C) 2014-2015  Ruby-GNOME2 Project Team
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

module Vte
  class Loader < GObjectIntrospection::Loader
    private
    def load_function_info(info)
      name = info.name
      case name
      when "init"
        # ignore
      else
        super
      end
    end

    def pre_load(repository, namespace)
      define_version_module
    end

    def define_version_module
      @version_module = Module.new
      @base_module.const_set("Version", @version_module)
    end

    def post_load(repository, namespace)
      require_libraries
    end

    def require_libraries
      require "vte3/pty"
      require "vte3/terminal"
      require "vte3/version"
      require "vte3/deprecated"
    end

    def load_constant_info(info)
      case info.name
      when /_VERSION\z/
        @version_module.const_set($PREMATCH, info.value)
      else
        super
      end
    end
    def initialize_post(object)
      super
      return unless object.is_a?(GLib::Object)
      self.class.reference_gobject(object, :sink => true)
    end
  end
end
