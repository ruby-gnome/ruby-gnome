# Copyright (c) 2003-2005 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
# $Id: hypertext.rb,v 1.2 2005/12/01 09:27:01 ktou Exp $
=begin
= Text Widget/Hypertext

Usually, tags modify the appearance of text in the view, e.g. making it
bold or colored or underlined. But tags are not restricted to appearance.
They can also affect the behavior of mouse and key presses, as this demo
shows.
=end

require 'common'

module Demo
  class Hypertext < BasicWindow
    def initialize
      super('Hypertext')

      @hand_cursor = Gdk::Cursor.new(Gdk::Cursor::HAND2)
      @regular_cursor = Gdk::Cursor.new(Gdk::Cursor::XTERM)
      @hovering = false

      set_default_size(450, 450)
      self.border_width = 0

      view = Gtk::TextView.new

      view.wrap_mode = Gtk::TextTag::WRAP_WORD

      view.signal_connect('key-press-event') do |*args|
        key_press_event(*args)
      end
      view.signal_connect('event-after') do |*args|
        event_after(*args)
      end
      view.signal_connect('motion-notify-event') do |*args|
        motion_notify_event(*args)
      end
      view.signal_connect('visibility-notify-event') do |*args|
        visibility_notify_event(*args)
      end

      buffer = view.buffer

      sw = Gtk::ScrolledWindow.new
      sw.set_policy(Gtk::POLICY_AUTOMATIC,
                    Gtk::POLICY_AUTOMATIC)

      add(sw)
      sw.add(view)

      show_page(buffer, 1)
    end


    # Inserts a piece of text into the buffer, giving it the usual
    # appearance of a hyperlink in a web browser: blue and underlined.
    # Additionally, attaches some data on the tag, to make it recognizable
    # as a link.
    def insert_link(buffer, iter, text, page)
      tag = buffer.create_tag(nil,
                              {
                                'foreground' => 'blue',
                                'underline' => Pango::AttrUnderline::SINGLE,
                              })
      tag.page = page

      buffer.insert(iter, text, tag)
      print("Insert #{tag}:#{page}\n")
    end

    # Fills the buffer with text and interspersed links. In any real
    # hypertext app, this method would parse a file to identify the links.
    def show_page(buffer, page)
      puts

      buffer.text = ''
      iter = buffer.get_iter_at_offset(0)

      case page
      when 1
        buffer.insert(iter, 'Some text ot show that simple ')
        insert_link(buffer, iter, 'hypertext', 3)
        buffer.insert(iter, ' can easily be realized with ')
        insert_link(buffer, iter, 'tags', 2)
        buffer.insert(iter, '.')
      when 2
        buffer.insert(iter,
                      %Q[A tag is an attribute that can be applied to some range of text. For example, a tag might be called "bold" and make the text insid the tag bold. However, the tag concept is more general than that; tags don't have to affect appearance. They can instead affect the behavior of mouse and key presses, "lock" a range of text so the user can't edit it, or countless other things.])
        buffer.insert(iter, "\n")
        insert_link(buffer, iter, 'Go back', 1)
      when 3
        tag = buffer.create_tag(nil,
                                {
                                  'weight' =>
                                    Pango::FontDescription::WEIGHT_BOLD
                                })
        buffer.insert(iter, "hypertext:\n", tag);
        buffer.insert(iter, %Q[machine-readable text that is not sequential but is organized so that related items of information are connected.\n])
        insert_link(buffer, iter, 'Go back', 1)
      end
    end

    # Looks at all tags covering the position of iter in the text view,
    # and if one of them is a link, follow it by showing the page identified
    # by the data attached to it.
    def follow_if_link(text_view, iter)
      tags = iter.tags
      tags.each do |tag|
        print("Follow #{tag}:#{tag.page}\n")
        if tag.page
          show_page(text_view.buffer, tag.page)
          break
        end
      end
    end

    # Links can be activated by pressing Enter.
    def key_press_event(text_view, event)
      case event.keyval
      when Gdk::Keyval::GDK_Return, Gdk::Keyval::GDK_KP_Enter
        buffer = text_view.buffer
        iter = buffer.get_iter_at_mark(buffer.get_mark("insert"))
        follow_if_link(text_view, iter)
      end

      false
    end

    # Links can also be activated by clicking.
    def event_after(text_view, event)
      unless event.kind_of?(Gdk::EventButton) and event.button == 1
        return false
      end

      buffer = text_view.buffer

      # we shouldn't follow a link if the user has selected something
      range = buffer.selection_bounds
      if range and range[0].offset != range[1].offset
        return false
      end

      x, y = text_view.window_to_buffer_coords(Gtk::TextView::WINDOW_WIDGET,
                                               event.x, event.y)
      iter = text_view.get_iter_at_location(x, y)

      follow_if_link(text_view, iter)
    end

    # Looks at all tags covering the position (x, y) in the text view,
    # and if one of them is a link, change the cursor to the "hands" cursor
    # typically used by web browsers.
    def set_cursor_if_appropriate(text_view, x, y)

      buffer = text_view.buffer
      iter = text_view.get_iter_at_location(x, y)

      hovering = false

      tags = iter.tags
      tags.each do |t|
        if t.page
          hovering = true
          break
        end
      end

      if hovering != @hovering
        @hovering = hovering

        window = text_view.get_window(Gtk::TextView::WINDOW_TEXT)

        window.cursor = if @hovering
                          @hand_cursor
                        else
                          @regular_cursor
                        end
      end
    end

    # Update the cursor image if the pointer moved.
    def motion_notify_event(text_view, event)
      x, y = text_view.window_to_buffer_coords(Gtk::TextView::WINDOW_WIDGET,
                                               event.x, event.y)
      set_cursor_if_appropriate(text_view, x, y)
      text_view.window.pointer

      false
    end

    # Also update the cursor image if the window becomes visible
    # (e.g. when a window covering it got iconified).
    def visibility_notify_event (text_view, event)
      window, wx, wy = text_view.window.pointer
      bx, by = text_view.window_to_buffer_coords(Gtk::TextView::WINDOW_WIDGET,
                                                 wx, wy)
      set_cursor_if_appropriate(text_view, bx, by)

      false
    end
  end
end

module Gtk
  class TextTag
    attr_accessor :page
  end
end
