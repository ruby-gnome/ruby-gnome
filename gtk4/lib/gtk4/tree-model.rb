# Copyright (C) 2014-2015  Ruby-GNOME2 Project Team
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
  module TreeModel
    alias_method :get_iter_raw, :get_iter
    def get_iter(path)
      if path.is_a?(String)
        got, iter = get_iter_from_string(path)
      else
        got, iter = get_iter_raw(path)
      end

      if got
        setup_iter(iter)
        iter
      else
        nil
      end
    end

    alias_method :iter_first_raw, :iter_first
    def iter_first
      got, iter = iter_first_raw
      if got
        setup_iter(iter)
        iter
      else
        nil
      end
    end

    alias_method :iter_parent_raw, :iter_parent
    def iter_parent(iter)
      got, iter = iter_parent_raw(iter)
      if got
        setup_iter(iter)
        iter
      else
        nil
      end
    end

    alias_method :get_value_raw, :get_value
    def get_value(iter, column)
      get_value_raw(iter, column).value
    end

    alias_method :iter_nth_child_raw, :iter_nth_child
    def iter_nth_child(iter, n)
      got, iter = iter_nth_child_raw(iter, n)
      if got
        setup_iter(iter)
        iter
      else
        nil
      end
    end

    alias_method :iter_children_raw, :iter_children
    def iter_children(iter)
      got, iter = iter_children_raw(iter)
      if got
        setup_iter(iter)
        iter
      else
        nil
      end
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

    alias_method :get_column_type_raw, :get_column_type
    def get_column_type(index)
      (@column_types ||= {})[index] ||= get_column_type_raw(index)
    end

    alias_method :create_filter, :filter_new

    private
    def setup_iter(iter)
      iter.model = self
    end
  end
end
