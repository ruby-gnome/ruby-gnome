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
          Gdk::Event.const_set($POSTMATCH, info.value)
        end
      end
    end

    def require_libraries
      require "gdk3/color"
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
        define_rectangle_method(info, $POSTMATCH)
      when /\Apixbuf_/
        define_pixbuf_singleton_method(info, $POSTMATCH)
      else
        super
      end
    end

    def define_pixbuf_singleton_method(function_info, name)
      target_module = Gdk::Pixbuf
      unlock_gvl = should_unlock_gvl?(function_info, target_module)
      validate = lambda do |arguments|
        validate_arguments(function_info, "#{target_module}.#{name}", arguments)
      end
      singleton_class = (class << target_module; self; end)
      singleton_class.__send__(:define_method, name) do |*arguments, &block|
        validate.call(arguments, &block)
        if block.nil? and function_info.require_callback?
          Enumerator.new(self, name, *arguments)
        else
          function_info.invoke({
                                 :arguments => arguments,
                                 :unlock_gvl => unlock_gvl,
                               },
                               &block)
        end
      end
    end

    def define_rectangle_method(function_info, name)
      target_module = Gdk::Rectangle
      unlock_gvl = should_unlock_gvl?(function_info, target_module)
      validate = lambda do |arguments|
        method_name = "#{target_module}\##{name}"
        validate_arguments(function_info, method_name, arguments)
      end
      target_module.module_eval do
        define_method(name) do |*arguments, &block|
          arguments = [self] + arguments
          validate.call(arguments, &block)
          function_info.invoke({
                                 :arguments => arguments,
                                 :unlock_gvl => unlock_gvl,
                               },
                               &block)
        end
      end
    end

    def load_struct_info(info)
      return if info.gtype_struct?

      options = {}
      case info.name
      when /\AEvent/
        options[:parent] = Gdk::Event
      end

      define_struct(info, options)
    end

    def define_enum(info)
      case info.name
      when /\AWindowWindow/
        self.class.define_class(info.gtype, $POSTMATCH, Gdk::Window)
      when /\AWindow/
        self.class.define_class(info.gtype, $POSTMATCH, Gdk::Window)
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
