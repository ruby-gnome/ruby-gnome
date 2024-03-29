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

module GOffice
  class Loader < GObjectIntrospection::Loader
    private
    def pre_load(repository, namespace)
    end

    def post_load(repository, namespace)
      define_version(repository, namespace)
      require_libraries
    end

    def define_version(repository, namespace)
      major, minor, micro = repository.get_version(namespace).split(".")
      version_module = Module.new do
        const_set("MAJOR", Integer(major))
        const_set("MINOR", Integer(minor))
        micro = Integer(micro) unless micro.nil?
        const_set("MICRO", micro)
      end
      @base_module.const_set("Version", version_module)
    end

    def require_libraries
      require "goffice/version"
    end

    def load_enum_value(value_info, enum_module)
      name = case value_info.name.upcase
      when /\A30E_360\z/
        "EUR_30_360"
      when /\A30EP_360\z/
        "EP_30_360"
      else
        value_info.name.upcase
      end
      enum_module.const_set(name, value_info.value)
    end

    def load_info(info)
      case info.name
      when "_SearchReplace"
        return
      when "gViewAllocation"
        return
      else
        super
      end
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

    def rubyish_class_name(info)
      name = super
      case name
      when /\A3DRotationSel\z/
        "Rotation3DSel"
      when /\Ag(\d*[A-Z]\w*)\z/
        "Graph#{Object::Regexp.last_match[1]}"
      when /\Ac(\d*[A-Z]\w*)\z/
        "Canvas#{Object::Regexp.last_match[1]}"
      when /\Ago_regression_stat_t\z/
        "RegressionStatT"
      when /\Ago_regression_stat_tl\z/
        "RegressionStatTl"
      when /\A\_(.*)\z/
        Object::Regexp.last_match[1]
      else
        name
      end
    end
  end
end
