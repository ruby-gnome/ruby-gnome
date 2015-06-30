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

    def set_values(iter, values)
      columns = []
      _values = []
      if values.is_a?(Hash)
        values.each do |column_id, value|
          type = get_column_type(column_id)
          columns << column_id
          _values << GLib::Value.new(type, value)
        end
      else
        values.each_with_index do |value, i|
          type = get_column_type(i)
          columns << i
          _values << GLib::Value.new(type, value)
        end
      end
      set(iter, columns, _values)
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
    def insert(index)
      iter = insert_raw(index)
      setup_iter(iter)
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
  end
end
