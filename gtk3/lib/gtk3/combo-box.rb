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
  class ComboBox
    alias_method :initialize_raw, :initialize
    def initialize(options={})
      entry = options[:entry]
      model = options[:model]
      area  = options[:area]

      if entry
        if model
          initialize_new_with_model_and_entry(model)
        elsif area
          initialize_new_with_area_and_entry(area)
        else
          initialize_new_with_entry
        end
      else
        if model
          initialize_new_with_model(model)
        elsif area
          initialize_new_with_area(area)
        else
          initialize_new
        end
      end
    end

    alias_method :active_iter_raw, :active_iter
    def active_iter
      found, iter = active_iter_raw
      if found
        iter.model = model
        iter
      else
        nil
      end
    end
  end
end
