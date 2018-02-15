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
  class SelectionData
    alias_method :set_raw, :set
    def set(type, data, options={})
      format ||= options[:format]
      if format.nil?
        case type
        when Gdk::Selection::TYPE_INTEGER
          format = 32
        when Gdk::Selection::TYPE_STRING
          format = 8
        else
          message = "specify :format as the number of bits of each data"
          raise ArgumentError, message
        end
      end
      set_raw(type, format, data)
    end

    alias_method :set_text_raw, :set_text
    def set_text(text)
      set_text_raw(text, text.bytesize)
    end
    alias_method :text=, :set_text
  end
end
