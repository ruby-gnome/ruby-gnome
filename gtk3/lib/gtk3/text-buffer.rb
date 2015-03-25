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
        message =
          "Must specify one of :line, :offset, :mark or :anchor: #{arguments.inspect}"
        raise ArgumentError, message
      end
    end

    alias_method :insert_raw,              :insert
    alias_method :insert_pixbuf_raw,       :insert_pixbuf
    alias_method :insert_child_anchor_raw, :insert_child_anchor
    def insert(iter, target, options={})
      interactive = options[:interactive]
      default_editable = options[:default_editable]
      tags = options[:tags]

      start_offset = iter.offset
      if interactive
        default_editable = true if default_editable.nil?
        insert_interactive(iter, target, default_editable)
      else
        case target
        when Gdk::Pixbuf
          insert_pixbuf_raw(iter, target)
        when TextChildAnchor
          insert_text_child_anchor_raw(iter, target)
        else
          insert_raw(iter, text, text.bytesize)
        end
      end

      if tags
        start_iter = get_iter_at(:offset => start_offset)
        tags.each do |tag|
          tag = tag_table.lookup(tag) if tag.is_a?(String)
          apply_tag(tag, start_iter, iter)
        end
      end

      self
    end

    alias_method :insert_at_cursor_raw, :insert_at_cursor
    def insert_at_cursor(text, options={})
      interactive = options[:interactive]
      default_editable = options[:default_editable]

      if interactive
        default_editable = true if default_editable.nil?
        insert_interactive_at_cursor(text, default_editable)
      else
        insert_at_cursor_raw(text, text.bytesize)
      end
    end

    alias_method :set_text_raw, :set_text
    def set_text(text)
      set_text_raw(text, text.bytesize)
    end
    remove_method :text=
    alias_method :text=, :set_text

    alias_method :apply_tag_raw, :apply_tag
    def apply_tag(tag, start, last)
      if tag.is_a?(String)
        apply_tag_by_name(tag, start, last)
      else
        apply_tag_raw(tag, start, last)
      end
    end

    alias_method :serialize_formats_raw, :serialize_formats
    def serialize_formats
      serialize_formats_raw[0]
    end

    private
    alias_method :insert_interactive_raw, :insert_interactive
    def insert_interactive(iter, text, default_ediatable)
      insert_interactive_raw(iter, text, text.bytesize, default_ediatable)
    end

    alias_method :insert_interactive_at_cursor_raw, :insert_interactive_at_cursor
    def insert_interactive_at_cursor(text, default_ediatable)
      insert_interactive_at_cursor_raw(text, text.bytesize, default_ediatable)
    end
  end
end
