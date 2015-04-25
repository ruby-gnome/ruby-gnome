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
  class UIManager
    alias_method :add_ui_raw, :add_ui
    def add_ui(buffer_or_filename, *args)
      if args.empty?
        if buffer_or_filename =~ /<ui>/
          add_ui_from_string(buffer_or_filename,
                             buffer_or_filename.length)
        else
          add_ui_from_file(buffer_or_filename)
        end
      else
        merge_id = buffer_or_filename
        path, name, action, type, top, = args
        add_ui_raw(merge_id, path, name, action, type, top)
      end
    end
  end
end
