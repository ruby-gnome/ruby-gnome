# Copyright (C) 2016-2021  Ruby-GNOME Project Team
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

module Gnm
  class Loader < GObjectIntrospection::Loader
    private
    def pre_load(repository, namespace)
    end

    def post_load(repository, namespace)
      require_libraries
    end

    def require_libraries
    end

    def load_enum_info(info)
      case info.name
      when "signtest_type", "ttest_type", /_t\z/
        # Ignore enum that don't support introspection
        return
      end
      super
    end

    def load_enum_value(value_info, enum_module)
      name = value_info.name.upcase
      case name
      when "3D_NAME"
        name = "NAME_3D"
      end
      enum_module.const_set(name, value_info.value)
    end

    def load_flags_info(info)
      case info.name
      when /_t\z/
        # Ignore flags that don't support introspection
        return
      end
      super
    end

    def rubyish_method_name(function_info, options={})
      name = super
      case name
      when /\A\d/
        "_#{$POSTMATCH}"
      else
        name
      end
    end
  end
end
