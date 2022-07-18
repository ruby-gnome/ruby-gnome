# Copyright (C) 2013-2022  Ruby-GNOME Project Team
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
    def event_class
      @event_class ||= @base_module.const_get(:Event)
    end

    def pre_load(repository, namespace)
      setup_pending_constants
      setup_pending_rectangle_functions
      define_keyval_module
      define_event_methods_module
      define_cairo_context_methods_module
      define_cairo_surface_methods_module
      require_pre_libraries
    end

    def define_keyval_module
      @keyval_module = define_methods_module(:Keyval)
    end

    def post_keyval_module
      post_methods_module(@keyval_module)
    end

    def define_event_methods_module
      @event_methods_module =
        define_methods_module(:EventMethods)
    end

    def apply_event_methods
      apply_methods_module(@event_methods_module,
                           event_class)
    end

    def define_cairo_context_methods_module
      @cairo_context_methods_module =
        define_methods_module(:CairoContextMethods)
    end

    def apply_cairo_context_methods
      apply_methods_module(@cairo_context_methods_module, Cairo::Context)
    end

    def define_cairo_surface_methods_module
      @cairo_surface_methods_module =
        define_methods_module(:CairoSurfaceMethods)
    end

    def apply_cairo_surface_methods
      apply_methods_module(@cairo_surface_methods_module, Cairo::Surface)
    end

    def require_pre_libraries
    end

    def post_load(repository, namespace)
      apply_pending_constants
      apply_pending_rectangle_functions
      post_keyval_module
      apply_event_methods
      apply_cairo_context_methods
      apply_cairo_surface_methods
      require_post_libraries
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

    def setup_pending_rectangle_functions
      @pending_rectangle_get_type_function = nil
      @pending_rectangle_functions = []
    end

    def apply_pending_rectangle_functions
      get_type_info = @pending_rectangle_get_type_function
      if get_type_info
        rectangle_class = load_cairo_rectangle_int
        gtype = get_type_info.invoke([])
        self.class.register_boxed_class_converter(gtype) do |rectangle|
          rectangle_class
        end
      else
        rectangle_class = @base_module.const_get(:Rectangle)
      end

      @pending_rectangle_functions.each do |info|
        name = rubyish_method_name(info, :prefix => "rectangle_")
        define_method(info, rectangle_class, name)
      end
    end

    def load_cairo_rectangle_int
      info = find_cairo_rectangle_int_info
      klass = self.class.define_class(info.gtype,
                                      "Rectangle",
                                      @base_module,
                                      :size => info.size)
      load_fields(info, klass)
      load_methods(info, klass)
      klass
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

    def require_post_libraries
      require_relative "cursor"
      require_relative "paintable"
      require_relative "rectangle"
      require_relative "rgba"
      require_relative "texture"

      require_relative "cairo"

      require_relative "deprecated"
    end

    def load_function_info(info)
      name = info.name
      case name
      when "rectangle_get_type"
        @pending_rectangle_get_type_function = info
      when /\Arectangle_/
        @pending_rectangle_functions << info
      when "init", /_get_type\z/
        # ignore
      when /\Apixbuf_/
        target_class = nil
        case $POSTMATCH
        when "get_from_surface"
          target_class = @cairo_surface_methods_module
        end
        if target_class
          define_method(info, target_class, "to_pixbuf")
        else
          super
        end
      when /\Aevents_/
        method_name = rubyish_method_name(info, :prefix => "events_")
        define_method(info, @event_methods_module, method_name)
      when /\Acairo_/
        name = $POSTMATCH
        case name
        when "set_source_color"
          define_method(info,
                        @cairo_context_methods_module,
                        "set_source_gdk_color")
        when "set_source_rgba"
          define_method(info,
                        @cairo_context_methods_module,
                        "set_source_gdk_rgba")
        when "rectangle"
          define_method(info,
                        @cairo_context_methods_module,
                        "gdk_rectangle")
        when "region_create_from_surface"
          # TODO
        else
          define_method(info, @cairo_context_methods_module, name)
        end
      when /\Akeyval_/
        name = rubyish_method_name(info, :prefix => "keyval_")
        case name
        when "name"
          name = "to_name"
        end
        define_module_function(@keyval_module, name, info)
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

    def may_be_cached?(constructor_info, klass)
      case klass.name
      when "Gdk::Cursor"
        case constructor_info.name
        when "new_from_name", "new_for_display"
          true
          # false
        else
          false
        end
      else
        false
      end
    end
  end
end
