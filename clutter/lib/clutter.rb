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
      require "clutter/actor-iter"
      require "clutter/cairo"
      require "clutter/color"
    end
  end

  class Loader < GObjectIntrospection::Loader
    def initialize(base_module, init_arguments)
      super(base_module)
      @init_arguments = init_arguments
      @key_constants = {}
      @other_constant_infos = []
    end

    private
    def pre_load(repository, namespace)
      init = repository.find(namespace, "init")
      init.invoke(@init_arguments.size, @init_arguments)
      @keys_module = Module.new
      @base_module.const_set("Keys", @keys_module)
      @threads_module = Module.new
      @base_module.const_set("Threads", @threads_module)
    end

    def post_load(repository, namespace)
      @other_constant_infos.each do |constant_info|
        name = constant_info.name
        next if @key_constants.has_key?("KEY_#{name}")
        @base_module.const_set(name, constant_info.value)
      end
    end

    def load_function_info(info)
      name = info.name
      case name
      when /\Athreads_/
        define_module_function(@threads_module, $POSTMATCH, info)
      else
        super
      end
    end

    def load_constant_info(info)
      case info.name
      when /\AKEY_/
        @key_constants[info.name] = true
        @keys_module.const_set(info.name, info.value)
      else
        @other_constant_infos << info
      end
    end
  end
end
