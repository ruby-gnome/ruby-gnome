# Copyright (C) 2015  Ruby-GNOME2 Project Team
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

module Gtk
  class IconView
    alias_method :initialize_raw, :initialize
    def initialize(options={})
      model = options[:model]
      area  = options[:area]

      if model
        initialize_new_with_model(model)
      elsif area
        initialize_new_with_area(area)
      else
        initialize_new
      end
    end

    if method_defined?(:get_cell_rect)
      alias_method :get_cell_rect_raw, :get_cell_rect
      def get_cell_rect(path, cell=nil)
        exist, rect = get_cell_rect_raw(path, cell)
        if exist
          rect
        else
          nil
        end
      end
    end
  end
end
