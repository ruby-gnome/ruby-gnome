# Copyright (C) 2014  Ruby-GNOME2 Project Team
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
  class Stack
    if method_defined?(:set_visible_child)
      alias_method :set_visible_child_raw, :set_visible_child
      def set_visible_child(widget_or_name, transition_type=nil)
        case widget_or_name
        when String
          name = widget_or_name
        else
          widget = widget_or_name
        end

        if widget
          set_visible_child_raw(widget)
        else
          if transition_type
            set_visible_child_full(name, transition_type)
          else
            set_visible_child_name(name)
          end
        end
      end
    end

    if method_defined?(:add_titled) and method_defined?(:add_named)
      def add(widget, name=nil, title=nil)
        if title
          add_titled(widget, name, title)
        elsif name
          add_named(widget, name)
        else
          super(widget)
        end
        self
      end
    end
  end
end
