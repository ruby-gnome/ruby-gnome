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
  class TreeStore
    alias_method :initialize_raw, :initialize
    def initialize(*column_types)
      # TODO: the following error occurred:
      # /.../gobject-introspection/lib/gobject-introspection/loader.rb:275:in `invoke': TODO: Ruby -> GIArgument(array)[GType] (NotImplementedError)
      initialize_raw(column_types.size, column_types)
    end

    def get_iter(path)
      if path.is_a?(String)
        # TODO: fix the method name "iter_from_string?".
        was_set, iter = iter_from_string?(path)
      else
        # TODO: fix the method name "iter?".
        was_set, iter = iter?(path)
      end

      if was_set
        iter
      else
        nil
      end
    end
  end
end
