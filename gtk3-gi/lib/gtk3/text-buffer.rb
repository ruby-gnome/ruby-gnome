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
    alias_method :initialize_raw, :initialize
    def initialize(table=nil)
      initialize_raw(table)
    end

    def create_tag(tag_name=nil, properties={})
      tag = Gtk::TextTag.new(tag_name)
      properties.each do |property|
        key, value = property
        tag.set_property(key, value)
      end
      tag_table.add(tag)
    end

    # prevent collision with deprecated methods.
    alias_method :_get_iter_at_line_offset,  :get_iter_at_line_offset
    alias_method :_get_iter_at_line_index,   :get_iter_at_line_index
    alias_method :_get_iter_at_line,         :get_iter_at_line
    alias_method :_get_iter_at_offset,       :get_iter_at_offset
    alias_method :_get_iter_at_mark,         :get_iter_at_mark
    alias_method :_get_iter_at_child_anchor, :get_iter_at_child_anchor
    private :_get_iter_at_line_offset
    private :_get_iter_at_line_index
    private :_get_iter_at_line
    private :_get_iter_at_offset
    private :_get_iter_at_mark
    private :_get_iter_at_child_anchor
    def get_iter_at(arguments)
      line   = arguments[:line]
      offset = arguments[:offset]
      index  = arguments[:index]
      mark   = arguments[:mark]
      anchor = arguments[:anchor]
      if line
        if offset
          _get_iter_at_line_offset(line, offset)
        elsif index
          _get_iter_at_line_index(line, index)
        else
          _get_iter_at_line(line)
        end
      elsif offset
        _get_iter_at_offset(offset)
      elsif mark
        _get_iter_at_mark(mark)
      elsif anchor
        _get_iter_at_child_anchor(anchor)
      else
        raise ArgumentError, "Invalid arguments."
      end
    end

    alias_method :insert_raw, :insert
    def insert(iter, text)
      insert_raw(iter, text, text.size)
    end
  end
end
