# Copyright (C) 2017-2021  Ruby-GNOME Project Team
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

module PangoCairo
  class Loader < GObjectIntrospection::Loader
    private
    def pre_load(repository, namespace)
      @context_cairo_methods_module =
        define_methods_module(:ContextCairoMethods)
      @cairo_context_methods_module =
        define_methods_module(:CairoContextMethods)
    end

    def post_load(repository, namespace)
      font_map_class = @base_module::FontMap
      font_types = []
      font_types << :quartz if Cairo::FontFace.quartz_supported?
      font_types << :win32 if Cairo::Surface.quartz_supported?
      font_types << :ft if Cairo::FontFace.freetype_supported?
      font_types.each do |font_type|
        font_map = font_map_class.new_for_font_type(font_type)
        next if font_map.nil?
        name = font_map.class.gtype.name.gsub(/\APangoCairo/, "")
        next if @base_module.const_defined?(name)
        @base_module.const_set(name, font_map.class)
      end
      apply_methods_module(@context_cairo_methods_module, Pango::Context)
      apply_methods_module(@cairo_context_methods_module, Cairo::Context)

      @base_module.constants.each do |constant|
        case constant
        when :INVOKERS,
             :Loader
          next
        else
          value = @base_module.const_get(constant)
          next if value == @context_cairo_methods_module
          next if value == @cairo_context_methods_module
          Pango.const_set("Cairo#{constant}", value)
        end
      end
    end

    def load_function_info(info)
      case info.name
      when /\Acontext_get_(.+)\z/
        method_name = $1
        define_method(info, @context_cairo_methods_module, method_name)
      when /\Acontext_set_(.+)\z/
        method_base_name = $1
        setter_method_name = "set_#{method_base_name}"
        define_method(info, @context_cairo_methods_module, setter_method_name)
        if info.n_args == 2
          equal_method_name = "#{method_base_name}="
          @context_cairo_methods_module.__send__(:alias_method,
                                                 equal_method_name,
                                                 setter_method_name)
        end
      when /\Afont_map_/
        # ignore
      when /\Acreate_(.+)\z/
        define_method(info, @cairo_context_methods_module, "create_pango_#{$1}")
      when /\A(.+_path)\z/
        define_method(info, @cairo_context_methods_module, "pango_#{$1}")
      when /\Ashow_(.+)\z/
        define_method(info, @cairo_context_methods_module, "show_pango_#{$1}")
      when /\Aupdate_(.+)\z/
        define_method(info, @cairo_context_methods_module, "update_pango_#{$1}")
      else
        warn("Unsupported PangoCairo function: #{info.name}")
      end
    end
  end
end
