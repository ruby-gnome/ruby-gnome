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
  class TextBuffer
    def create_tag(tag_name=nil, properties={})
      tag = TextTag.new(tag_name)
      properties.each do |property|
        key, value = property
        tag.set_property(key, value)
      end
      tag_table.add(tag)
    end

    # prevent collision with deprecated methods.
    alias_method :get_iter_at_line_offset_raw,  :get_iter_at_line_offset
    alias_method :get_iter_at_line_index_raw,   :get_iter_at_line_index
    alias_method :get_iter_at_line_raw,         :get_iter_at_line
    alias_method :get_iter_at_offset_raw,       :get_iter_at_offset
    alias_method :get_iter_at_mark_raw,         :get_iter_at_mark
    alias_method :get_iter_at_child_anchor_raw, :get_iter_at_child_anchor
    def get_iter_at(arguments)
      line   = arguments[:line]
      offset = arguments[:offset]
      index  = arguments[:index]
      mark   = arguments[:mark]
      anchor = arguments[:anchor]
      if line
        if offset
          get_iter_at_line_offset_raw(line, offset)
        elsif index
          get_iter_at_line_index_raw(line, index)
        else
          get_iter_at_line_raw(line)
        end
      elsif offset
        get_iter_at_offset_raw(offset)
      elsif mark
        get_iter_at_mark_raw(mark)
      elsif anchor
        get_iter_at_child_anchor_raw(anchor)
      else
        raise ArgumentError, "Invalid arguments."
      end
    end

    alias_method :insert_raw, :insert
    def insert(iter, text)
      insert_raw(iter, text, text.bytesize)
    end

    alias_method :apply_tag_raw, :apply_tag
    def apply_tag(tag, start, last)
      if tag.is_a?(String)
        apply_tag_by_name(tag, start, last)
      else
        apply_tag_raw(tag, start, last)
      end
    end
  end
end
