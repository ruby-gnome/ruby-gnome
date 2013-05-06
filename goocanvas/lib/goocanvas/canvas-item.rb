# Copyright (C) 2013  Ruby-GNOME2 Project Team
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

module Goo
  module CanvasItem
    def initialize(*args, &block)
      super
      parent.__send__(:add_child_reference, self) if parent
    end

    alias_method :add_child_raw, :add_child
    private :add_child_raw
    def add_child(child)
      result = add_child_raw(child)
      add_child_reference(child)
      result
    end

    alias_method :remove_child_raw, :remove_child
    private :remove_child_raw
    def remove_child(child)
      result = remove_child_raw(child)
      remove_child_reference(child)
      result
    end

    private
    def add_child_reference(child)
      @children ||= []
      @children << child
    end

    def remove_child_reference(child)
      @children ||= []
      @children.delete(child)
    end
  end
end
