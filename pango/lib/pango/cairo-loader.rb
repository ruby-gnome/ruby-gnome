# Copyright (C) 2017  Ruby-GNOME2 Project Team
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

module Pango
  class CairoLoader < GObjectIntrospection::Loader
    private
    def pre_load(repository, namespace)
      @context_class = @base_module.const_get(:Context)
    end

    def rubyish_class_name(info)
      "Cairo#{super}"
    end

    def load_function_info(info)
      case info.name
      when /\Acontext_get_(.+)\z/
        method_name = $1
        define_method(info, @context_class, method_name)
      when /\Acontext_set_(.+)\z/
        method_base_name = $1
        setter_method_name = "set_#{method_base_name}"
        if @context_class.method_defined?(setter_method_name)
          # ignore
          return
        end
        define_method(info, @context_class, setter_method_name)
        if info.n_args == 2
          equal_method_name = "#{method_base_name}="
          remove_existing_method(@context_class, equal_method_name)
          @context_class.__send__(:alias_method,
                                  equal_method_name,
                                  setter_method_name)
        end
      when /\Afont_map_/
        # ignore
      when /\Acreate_(.+)\z/
        define_method(info, Cairo::Context, "create_pango_#{$1}")
      when /\A(.+_path)\z/
        define_method(info, Cairo::Context, "pango_#{$1}")
      when /\Ashow_(.+)\z/
        define_method(info, Cairo::Context, "show_pango_#{$1}")
      when /\Aupdate_(.+)\z/
        define_method(info, Cairo::Context, "update_pango_#{$1}")
      else
        warn("Unsupported PangoCairo function: #{info.name}")
      end
    end
  end
end
