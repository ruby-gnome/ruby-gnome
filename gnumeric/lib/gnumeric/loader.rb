# Copyright (C) 2016 Ruby-GNOME2 Project Team
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

module Gnumeric 
  class Loader < GObjectIntrospection::Loader
    private
    def load_function_info(info)
      name = info.name
      case name
      when "init"
        # ignore
      else
        super
      end
    end

    def pre_load(repository, namespace)
    end

    def post_load(repository, namespace)
      require_libraries
    end

    def require_libraries
    end

    def load_enum_value(value_info, enum_module)
      name = case value_info.name.upcase
      when /\A3D_NAME\z/
        "NAME_3D"
      else
        value_info.name.upcase
      end
      enum_module.const_set(name, value_info.value)
    end

    def load_enum_info(info)
      if info.gtype == GLib::Type::NONE
        enum_module = Module.new
        info.values.each do |value_info|
          load_enum_value(value_info, enum_module)
        end
        @base_module.const_set(rename_class(info.name), enum_module)
      else
        if info.error_domain
          define_error(info)
        else
          define_enum(info)
        end
      end
    end

    def initialize_post(object)
      super
      return unless object.is_a?(GLib::Object)
      self.class.reference_gobject(object, :sink => true)
    end
    
    def rename_class(class_name)
      case class_name
      when /\Aanalysis_tool_engine_t\z/
        "AnalysisToolEngine"
      when /\Adata_analysis_output_type_t\z/
        "DataAnalysisOutputType"
      when /\Agroup_by_t\z/
        "GroupBy"
      when /\Awb_control_navigator_t\z/
        "WbControlNavigator"
      else
        class_name
      end
    end  
  end
end
