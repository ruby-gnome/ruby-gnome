# Copyright (C) 2021  Ruby-GNOME Project Team
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

module PangoOT
  class Loader < GObjectIntrospection::Loader
    def post_load(repository, namespace)
      @base_module.constants.each do |constant|
        case constant
        when :INVOKERS,
             :Loader
          next
        else
          if constant.to_s.upcase == constant.to_s
            name = "OT_#{constant}"
          else
            name = "OT#{constant}"
          end
          value = @base_module.const_get(constant)
          Pango.const_set(name, value)
        end
      end
    end
  end
end
