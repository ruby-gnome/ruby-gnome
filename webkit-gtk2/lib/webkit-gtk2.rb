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

require 'gobject-introspection'
require 'gtk2'

base_dir = Pathname.new(__FILE__).dirname.dirname.dirname.expand_path
vendor_dir = base_dir + "vendor" + "local"
vendor_bin_dir = vendor_dir + "bin"
GLib.prepend_environment_path(vendor_bin_dir)

module WebKitGtk2
  @initialized = false
  class << self
    def init
      return if @initialized
      @initialized = true
      loader = Loader.new(self)
      loader.load("WebKit","1.0")
    end
  end

  class Loader < GObjectIntrospection::Loader
    def load(namespace,version=nil)
      repository = GObjectIntrospection::Repository.default
      repository.require(namespace,version)
      pre_load(repository,namespace)
      repository.each(namespace) do |info|
        load_info(info)
      end
      post_load(repository,namespace)
    end
  end
end
