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

module Gdk
  class Loader < GObjectIntrospection::Loader
    def load
      self.version = "4.0"
      super("Gdk")
    end

    private
    def pre_load(repository, namespace)
      define_version(repository, namespace)
    end

    def define_version(repository, namespace)
      major, minor, micro = repository.get_version(namespace).split(".")
      version_module = Module.new do
        const_set("MAJOR", Integer(major))
        const_set("MINOR", Integer(minor))
        micro = Integer(micro) unless micro.nil?
        const_set("MICRO", micro)
      end
      @base_module.const_set("Version", version_module)
    end

    def post_load(repository, namespace)
      require_libraries
    end

    def require_libraries
      # require "gdk4/atom"
      require "gdk4/rectangle"
      require "gdk4/rgba"
      require "gdk4/version"
    end
  end
end
