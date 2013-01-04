# Copyright (C) 2012  Ruby-GNOME2 Project Team
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
require "cairo-gobject"

base_dir = Pathname.new(__FILE__).dirname.dirname.dirname.expand_path
vendor_dir = base_dir + "vendor" + "local"
vendor_bin_dir = vendor_dir + "bin"
GLib.prepend_environment_path(vendor_bin_dir)

module Clutter
  LOG_DOMAIN = "Clutter"
  GLib::Log.set_log_domain(LOG_DOMAIN)

  class << self
    @initialized = false
    def init(argv=ARGV)
      return if @initialized
      @initialized = true
      loader = Loader.new(self, argv)
      loader.load("Clutter")
      require "clutter/actor"
      require "clutter/cairo"
      require "clutter/color"
    end
  end

  class Loader < GObjectIntrospection::Loader
    def initialize(base_module, init_arguments)
      super(base_module)
      @init_arguments = init_arguments
    end

    private
    def prepare(repository, namespace)
      init = repository.find(namespace, "init")
      init.invoke(@init_arguments.size, @init_arguments)
    end

    def load_constant_info(info)
      return unless /\A(?:KEY_|COLOR_)/ =~ info.name
      super
    end
  end
end
