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
  class ListStore
    alias_method :initialize_raw, :initialize
    def initialize(*columns)
      if columns.empty?
        raise ArgumentError, "No column type is specified"
      end
      initialize_raw(columns)
    end

    alias_method :append_raw, :append
    def append
      iter = append_raw
      setup_iter(iter)
      iter
    end

    alias_method :prepend_raw, :prepend
    def prepend
      iter = prepend_raw
      setup_iter(iter)
      iter
    end

    alias_method :insert_raw, :insert
    def insert(index, values=nil)
      iter = insert_raw(index)
      setup_iter(iter)
      set_values(iter, values) if values
      iter
    end

    alias_method :insert_before_raw, :insert_before
    def insert_before(index)
      iter = insert_before_raw(index)
      setup_iter(iter)
      iter
    end

    alias_method :insert_after_raw, :insert_after
    def insert_after(index)
      iter = insert_after_raw(index)
      setup_iter(iter)
      iter
    end

    alias_method :set_column_types_raw, :set_column_types
    def set_column_types(*types)
      if types.size == 1 and types[0].is_a?(Array)
        types = types[0]
      end
      set_column_types_raw(types)
    end
  end
end
