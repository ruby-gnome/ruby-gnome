# Copyright (C) 2014-2016 Ruby-GNOME2 Project Team
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
  class TreeIter
    def model
      @model
    end

    def model=(model)
      @model = model
    end

    def get_value(column)
      @model.get_value(self, column)
    end
    alias_method :[], :get_value

    def set_value(column, value)
      gtype = @model.get_column_type(column)
      gvalue = GLib::Value.new(gtype, value)
      @model.set_value(self, column, gvalue)
    end
    alias_method :[]=, :set_value

    def set_values(values)
      @model.set_values(self, values)
    end
    alias_method :values=, :set_values

    def path
      @model.get_path(self)
    end

    def previous!
      @model.iter_previous(self)
    end

    def next!
      @model.iter_next(self)
    end

    def parent
      @model.iter_parent(self)
    end

    def has_child?
      @model.iter_has_child(self)
    end

    def n_children
      @model.iter_n_children(self)
    end

    def nth_child(n)
      @model.iter_nth_child(self, n)
    end

    def children
      @model.iter_children(self)
    end

    def first_child
      nth_child(0)
    end

    def ==(other)
      other.is_a?(self.class) and
        @model == other.model and
        path == other.path
    end
  end
end
