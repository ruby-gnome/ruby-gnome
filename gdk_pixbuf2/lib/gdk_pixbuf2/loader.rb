# Copyright (C) 2016-2018  Ruby-GNOME2 Project Team
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

require "English"

module GdkPixbuf
  class Loader < GObjectIntrospection::Loader
    private
    def pre_load(repository, namespace)
    end

    def post_load(repository, namespace)
      require_libraries
    end

    def require_libraries
      require "gdk_pixbuf2/pixbuf"
      require "gdk_pixbuf2/pixbuf-loader"

      require "gdk_pixbuf2/deprecated"

      require "gdk_pixbuf2/version"
    end

    def define_constant(name, info)
      if /PIXBUF_/ =~ name
        name = $POSTMATCH
      end
      super(name, info)
    end

    def rubyish_method_name(function_info, options={})
      name = function_info.name
      if name == "new_subpixbuf"
        "subpixbuf"
      else
        super
      end
    end
  end
end
