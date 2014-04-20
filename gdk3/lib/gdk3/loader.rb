# Copyright (C) 2013-2014  Ruby-GNOME2 Project Team
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
    private
    def window_class
      @window_class ||= @base_module.const_get(:Window)
    end

    def event_class
      @event_class ||= @base_module.const_get(:Event)
    end

    def event_motion_class
      @event_motion_class ||= @base_module.const_get(:EventMotion)
    end

    def rectangle_class
      @rectangle_class ||= @base_module.const_get(:Rectangle)
    end

    def pre_load(repository, namespace)
      setup_pending_constants
      define_keyval_module
      load_cairo_rectangle_int
    end

    def define_keyval_module
      @keyval_module = Module.new
      @base_module.const_set("Keyval", @keyval_module)
    end

    def load_cairo_rectangle_int
      info = find_cairo_rectangle_int_info
      klass = self.class.define_class(info.gtype,
                                      "Rectangle",
                                      @base_module,
                                      :size => info.size)
      load_fields(info, klass)
      load_methods(info, klass)
    end

    def find_cairo_rectangle_int_info
      repository = GObjectIntrospection::Repository.default
      repository.each("cairo") do |info|
        if info.name == "RectangleInt"
          return info
        end
      end
      nil
    end

    def post_load(repository, namespace)
      apply_pending_constants
      require_libraries
    end

    def setup_pending_constants
      @pending_constants = []
    end

    def apply_pending_constants
      @pending_constants.each do |info|
        case info.name
        when /\AEVENT_/
          event_class.const_set($POSTMATCH, info.value)
        end
      end
    end

    def require_libraries
      require "gdk3/color"
      require "gdk3/event"
      require "gdk3/rectangle"
      require "gdk3/rgba"
      require "gdk3/window-attr"

      require "gdk3/deprecated"
    end

    def load_function_info(info)
      name = info.name
      case name
      when "init", /_get_type\z/
        # ignore
      when /\Arectangle_/
        define_method(info, rectangle_class, $POSTMATCH)
      when /\Apixbuf_/
        target_class = nil
        case $POSTMATCH
        when "get_from_window"
          target_class = window_class
        when "get_from_surface"
          target_class = Cairo::Surface
        end
        if target_class
          define_method(info, target_class, "to_pixbuf")
        else
          super
        end
      when /\Aevent_/
        name = $POSTMATCH
        case name
        when "request_motions"
          define_method(info, event_motion_class, "request")
        else
          super # TODO
        end
      else
        super
      end
    end

    def load_struct_info(info)
      return if info.gtype_struct?

      options = {}
      case info.name
      when /\AEvent/
        options[:parent] = event_class
      end

      define_struct(info, options)
    end

    def define_enum(info)
      case info.name
      when /\AWindowWindow/
        self.class.define_class(info.gtype, $POSTMATCH, window_class)
      when /\AWindow/
        self.class.define_class(info.gtype, $POSTMATCH, window_class)
      when "EventType"
        self.class.register_constant_rename_map("2BUTTON_PRESS",
                                                "BUTTON2_PRESS")
        self.class.register_constant_rename_map("3BUTTON_PRESS",
                                                "BUTTON3_PRESS")
        super
      else
        super
      end
    end

    def load_constant_info(info)
      case info.name
      when /\AEVENT_/
        @pending_constants << info
      when /\AKEY_/
        @keyval_module.const_set(info.name, info.value)
      else
        super
      end
    end
  end
end
