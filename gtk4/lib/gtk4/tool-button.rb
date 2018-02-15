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
  class ToolButton
    alias_method :initialize_raw, :initialize
    def initialize(options={})
      icon_widget = options[:icon_widget]
      label       = options[:label]
      stock_id    = options[:stock_id]

      if icon_widget
        initialize_new(icon_widget)
      elsif stock_id
        initialize_new_from_stock(stock_id)
      else
        initialize_new
      end

      set_label(label) if label
    end
  end
end
