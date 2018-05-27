# Copyright (C) 2014-2018  Ruby-GNOME2 Project Team
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
      succeeded = tag_table.add(tag)
      return nil unless succeeded

      properties.each do |name, value|
        property_name = name.to_s.gsub(/-/, "_")
        tag.__send__("#{property_name}=", value)
      end

      tag
    end

    alias_method :add_mark_raw, :add_mark
    def add_mark(mark, where)
      @marks ||= {}
      add_mark_raw(mark, where)
      mark_name = mark.name
      @marks[mark_name] = mark if mark_name
    end

    alias_method :create_mark_raw, :create_mark
    def create_mark(name, where, options={})
      if options == true or options == false
        options = {:left_gravity => options}
      end
      left_gravity = options[:left_gravity]
      left_gravity = true if left_gravity.nil?
      @marks ||= {}
      if name.nil?
        create_mark_raw(name, where, left_gravity)
      else
        @marks[name] = create_mark_raw(name, where, left_gravity)
      end
    end

    alias_method :delete_mark_raw, :delete_mark
    def delete_mark(mark)
      @marks ||= {}
      mark_name = mark.name
      delete_mark_raw(mark)
      @marks.delete(mark_name) if mark_name
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
    def insert(iter, target, *args)
      options = nil
      tags = nil
      case args.size
      when 0
        options = {}
      when 1
        case args.first
        when Hash
          options = args.first
        else
          tags = args
        end
      else
        tags = args
      end
      if options.nil?
        signature_prefix = "#{self.class}\##{__method__}(iter, target"
        warn("#{signature_prefix}, *tags) style has been deprecated. " +
             "Use #{signature_prefix}, options={:tags => tags}) style instead.")
        options = {:tags => tags}
      end

      interactive = options[:interactive]
      default_editable = options[:default_editable]
      tags = options[:tags]

      start_offset = iter.offset
      if interactive
        default_editable = true if default_editable.nil?
        insert_interactive(iter, target, default_editable)
      else
        case target
        when GdkPixbuf::Pixbuf
          insert_pixbuf_raw(iter, target)
        when TextChildAnchor
          insert_text_child_anchor_raw(iter, target)
        when GLib::Bytes
          insert_raw(iter, target, target.size)
        else
          insert_raw(iter, target, target.bytesize)
        end
      end

      if tags
        start_iter = get_iter_at(:offset => start_offset)
        tags.each do |tag|
          if tag.is_a?(String)
            resolved_tag = tag_table.lookup(tag)
            if resolved_tag.nil?
              raise ArgumentError "unknown tag: #{tag.inspect}"
            end
            tag = resolved_tag
          end
          apply_tag(tag, start_iter, iter)
        end
      end

      self
    end

    if method_defined?(:insert_markup)
      alias_method :insert_markup_raw, :insert_markup
      def insert_markup(iter, markup, n_bytes=nil)
        case markup
        when GLib::Bytes
          n_bytes ||= markup.size
        else
          n_bytes ||= markup.bytesize
        end
        insert_markup_raw(iter, markup, n_bytes)
      end
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
      if text.is_a?(GLib::Bytes)
        text, text_size = text.to_s, text.size
      else
        text_size = text.bytesize
      end
      set_text_raw(text, text_size)
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

    alias_method :serialize_raw, :serialize
    def serialize(*arguments)
      serialize_raw(*arguments).pack("C*")
    end

    alias_method :selection_bounds_raw, :selection_bounds
    def selection_bounds
      selected, start_iter, end_iter = selection_bounds_raw
      if selected
        [start_iter, end_iter]
      else
        nil
      end
    end

    private
    alias_method :insert_interactive_raw, :insert_interactive
    def insert_interactive(iter, text, default_ediatable)
      if text.is_a?(GLib::Bytes)
        text, text_size = text.to_s, text.size
      else
        text_size = text.bytesize
      end
      insert_interactive_raw(iter, text, text_size, default_ediatable)
    end

    alias_method :insert_interactive_at_cursor_raw, :insert_interactive_at_cursor
    def insert_interactive_at_cursor(text, default_ediatable)
      if text.is_a?(GLib::Bytes)
        text, text_size = text.to_s, text.size
      else
        text_size = text.bytesize
      end
      insert_interactive_at_cursor_raw(text, text_size, default_ediatable)
    end
  end
end
