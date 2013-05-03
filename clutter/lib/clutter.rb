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

require "gobject-introspection"
require "cairo-gobject"

base_dir = Pathname.new(__FILE__).dirname.dirname.expand_path
vendor_dir = base_dir + "vendor" + "local"
vendor_bin_dir = vendor_dir + "bin"
GLib.prepend_dll_path(vendor_bin_dir)
vendor_girepository_dir = vendor_dir + "lib" + "girepository-1.0"
GObjectIntrospection.prepend_typelib_path(vendor_girepository_dir)

if vendor_dir.exist?
  require "atk"
  require "pango"
  require "gdk_pixbuf2"
end

module Clutter
  LOG_DOMAIN = "Clutter"
  GLib::Log.set_log_domain(LOG_DOMAIN)

  @initialized = false
  class << self
    def init(argv=ARGV)
      return if @initialized
      @initialized = true
      loader = Loader.new(self, argv)
      loader.load("Clutter")
      require "clutter/actor"
      require "clutter/actor-iter"
      require "clutter/animatable"
      require "clutter/cairo"
      require "clutter/color"
      require "clutter/event"
      require "clutter/point"
      require "clutter/text"
      require "clutter/threads"
    end
  end

  class Loader < GObjectIntrospection::Loader
    class InitError < StandardError
    end

    def initialize(base_module, init_arguments)
      super(base_module)
      @init_arguments = init_arguments
      @key_constants = {}
      @other_constant_infos = []
      @event_infos = []
    end

    private
    def pre_load(repository, namespace)
      init = repository.find(namespace, "init")
      arguments = [
        1 + @init_arguments.size,
        [$0] + @init_arguments,
      ]
      error, argc, argv = init.invoke(arguments)
      @init_arguments.replace(argv)
      if error.to_i <= 0
        raise InitError, "failed to initialize Clutter: #{error.name}"
      end
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
      load_events
    end

    def load_events
      @event_infos.each do |event_info|
        define_struct(event_info, :parent => Event)
      end
      event_map = {
        EventType::KEY_PRESS      => KeyEvent,
        EventType::KEY_RELEASE    => KeyEvent,
        EventType::MOTION         => MotionEvent,
        EventType::ENTER          => CrossingEvent,
        EventType::LEAVE          => CrossingEvent,
        EventType::BUTTON_PRESS   => ButtonEvent,
        EventType::BUTTON_RELEASE => ButtonEvent,
        EventType::SCROLL         => ScrollEvent,
        EventType::STAGE_STATE    => StageStateEvent,
        EventType::TOUCH_UPDATE   => TouchEvent,
        EventType::TOUCH_END      => TouchEvent,
        EventType::TOUCH_CANCEL   => TouchEvent,
      }
      self.class.register_boxed_class_converter(Event.gtype) do |event|
        event_map[event.type] || Event
      end
    end

    def load_struct_info(info)
      if info.name.end_with?("Event")
        @event_infos << info
      else
        super
      end
    end

    def load_function_info(info)
      name = info.name
      case name
      when "init"
        # ignore
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
