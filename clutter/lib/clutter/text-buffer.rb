# Copyright (C) 2012-2014  Ruby-GNOME2 Project Team
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

module Clutter
  class TextBuffer
    alias_method :set_text_raw, :set_text
    alias_method :insert_text_raw, :insert_text
    alias_method :emit_inserted_text_raw, :emit_inserted_text

    def set_text(string)
      set_text_raw(string, string.length)
    end
    alias_method :text=, :set_text

    def insert_text(position, string)
      insert_text_raw(position, string, string.length)
    end

    def emit_inserted_text(position, string)
      emit_inserted_text_raw(position, string, string.length)
    end
  end
end
