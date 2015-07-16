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
  class TreeModelFilter
    def initialize(child_model, root=nil)
      super(:child_model => child_model, :virtual_root => root)
    end

    alias_method :set_modify_func_raw, :set_modify_func
    def set_modify_func(*types, &block)
      raise ArgumentError, "one or more types are required" if types.empty?
      set_modify_func_raw(types, &block)
    end

    alias_method :convert_iter_to_child_iter_raw, :convert_iter_to_child_iter
    def convert_iter_to_child_iter(iter)
      child_iter = convert_iter_to_child_iter_raw(iter)
      child_iter.model = model
      child_iter
    end

    alias_method :convert_child_iter_to_iter_raw, :convert_child_iter_to_iter
    def convert_child_iter_to_iter(child_iter)
      converted, iter = convert_child_iter_to_iter_raw(child_iter)
      return nil unless converted

      iter.model = self
      iter
    end
  end
end
