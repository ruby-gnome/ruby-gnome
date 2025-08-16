# Copyright (C) 2025  Ruby-GNOME Project Team
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

module Graphene
  class Loader < GObjectIntrospection::Loader
    def load
      self.version = "1.0"
      super("Graphene")
    end

    private
    def post_load(repository, namespace)
      require_relative "point"
      require_relative "rect"
      require_relative "size"
      require_relative "vec2"
    end

    def rubyish_method_name(function_info, options={})
      case function_info.name
      when "is_2d"
        "two_dimentional?"
      else
        super
      end
    end
  end
end
