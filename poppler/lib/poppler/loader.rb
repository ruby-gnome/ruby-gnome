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

module Poppler
  class Loader < GObjectIntrospection::Loader
    private
    def pre_load(repository, namespace)
      @form_field_classes = {}
    end

    def post_load(repository, namespace)
      require_libraries
      convert_field_classes
    end

    def require_libraries
      require "poppler/version"

      require "poppler/annot-callout-line"
      require "poppler/cairo"
      require "poppler/color"
      require "poppler/document"
      require "poppler/page"
      require "poppler/rectangle"

      require "poppler/deprecated"
    end

    def load_enum_info(info)
      case info.name
      when "AnnotType"
        self.class.register_constant_rename_map("3D", "TYPE_3D")
      when "AnnotExternalDataType"
        self.class.register_constant_rename_map("3D", "TYPE_3D")
      when "StructureGlyphOrientation"
        self.class.register_constant_rename_map("0", "DEGREE_0")
        self.class.register_constant_rename_map("90", "DEGREE_90")
        self.class.register_constant_rename_map("180", "DEGREE_180")
        self.class.register_constant_rename_map("270", "DEGREE_270")
      end
      super
    end

    def load_method_info(info, klass, method_name)
      case klass.name
      when "Poppler::Annot"
        case method_name
        when "annot_type"
          method_name = "type"
        end
      when "Poppler::FormField"
        case method_name
        when /\Abutton_/
          klass = define_field_class("ButtonField")
          method_name = rubyish_method_name(info, prefix: "button_")
          method_name = "type" if method_name == "button_type"
        when /\Atext_/
          klass = define_field_class("TextField")
          method_name = rubyish_method_name(info, prefix: "text_")
          method_name = "type" if method_name == "text_type"
        when /\Achoice_/
          klass = define_field_class("ChoiceField")
          method_name = rubyish_method_name(info, prefix: "choice_")
          method_name = "type" if method_name == "choice_type"
        end
      end
      super(info, klass, method_name)
    end

    def define_field_class(name)
      klass = @form_field_classes[name]
      return klass if klass

      field_class = @base_module.const_get("FormField")
      klass = @form_field_classes[name] = Class.new(field_class)
      @base_module.const_set(name, klass)
      klass
    end

    def convert_field_classes
      define_field_class("SignatureField")

      field_map = {
        FormFieldType::BUTTON    => ButtonField,
        FormFieldType::TEXT      => TextField,
        FormFieldType::CHOICE    => ChoiceField,
        FormFieldType::SIGNATURE => SignatureField,
      }
      self.class.register_object_class_converter(FormField.gtype) do |field|
        field_map[field.field_type] || FormField
      end
    end
  end
end
