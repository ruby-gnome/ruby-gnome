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
  class Loader < GObjectIntrospection::Loader

    private

    def pre_load(repository, namespace)
      @pending_attribute_infos = []
    end

    def post_load(repository, namespace)
      @pending_attribute_infos.each do |info|
        define_struct(info, :parent => Attribute)
      end
      require_libraries
    end

    def load_struct_info(info)
      case info.name
      when /Class\z/
        super
      when/\AAttr[A-Z]/
        @pending_attribute_infos << info
      else
        super
      end
    end

    def load_method_info(info, klass, method_name)
      case klass.name
      when "Pango::Matrix"
        case method_name
        when "translate", "scale", "rotate", "concat"
          method_name += "!"
        end
      end
      super(info, klass, method_name)
    end

    def require_libraries
      require "pango/attr-language"
      require "pango/color"
      require "pango/font-description"
      require "pango/language"
      require "pango/layout"
      require "pango/rectangle"
      require "pango/matrix"
      require "pango/version"

      require "pango/deprecated"
    end
  end
end
