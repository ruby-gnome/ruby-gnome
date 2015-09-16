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
  class Table
    alias_method :initialize_raw, :initialize
    def initialize(n_rows, n_columns, homogeneous=false)
      initialize_raw(n_rows, n_columns, homogeneous)
    end

    alias_method :default_column_spacing, :default_col_spacing

    def column_spacings(column)
      get_col_spacings(column)
    end
    alias_method :set_column_spacings, :set_col_spacings
    alias_method :column_spacings=,    :col_spacings=

    alias_method :attach_raw, :attach
    def attach(child, left, right, top, bottom,
               x_options=nil, y_options=nil,
               x_space=nil, y_space=nil)
      attach_raw(child, left, right, top, bottom,
                 x_options || [:expand, :fill],
                 y_options || [:expand, :fill],
                 x_space || 0,
                 y_space || 0)
    end
  end
end
