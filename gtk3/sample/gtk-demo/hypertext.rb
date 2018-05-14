# Copyright (c) 2016 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
=  Text View/Hypertext

 Usually, tags modify the appearance of text in the view, e.g. making it
 bold or colored or underlined. But tags are not restricted to appearance.
 They can also affect the behavior of mouse and key presses, as this demo
 shows.
=end
class HypertextDemo
  def initialize(main_window)
    @hovering = false

    initialize_window(main_window)
    initialize_cursors(main_window)
    initialize_view

    @view.signal_connect "key-press-event" do |_widget, event|
      case event.keyval
      when Gdk::Keyval::KEY_Return, Gdk::Keyval::KEY_KP_Enter
        iter = @buffer.get_iter_at_mark(@buffer.get_mark("insert"))
        follow_if_link(iter) if iter
      end

      false
    end

    @view.signal_connect "event-after" do |widget, event|
      event_after(event)
    end

    @view.signal_connect "motion-notify-event" do |widget, event|
      x, y = widget.window_to_buffer_coords(:widget, event.x, event.y)
      set_cursor_if_appropriate(widget, x, y)
      widget.window.pointer

      false
    end

    sw = Gtk::ScrolledWindow.new
    sw.set_policy(:automatic, :automatic)
    @window.add(sw)
    sw.add(@view)
    show_page(1)
    sw.show_all
  end

  def run
    if !@window.visible?
      @window.show_all
    else
      @window.destroy
    end
    @window
  end

  private

  def initialize_window(main_window)
    @window = Gtk::Window.new(:toplevel)
    @window.screen = main_window.screen
    @window.title = "Hypertext"
    @window.set_default_size(450, 450)
  end

  def initialize_cursors(main_window)
    display = main_window.display
    @hand_cursor = Gdk::Cursor.new("pointer", :display => display)
    @regular_cursor = Gdk::Cursor.new("text", :display => display)
  end

  def initialize_view
    @view = Gtk::TextView.new
    @view.wrap_mode = :word
    @view.left_margin = 20
    @view.right_margin = 20
    @buffer = @view.buffer
  end

  # Fills the buffer with text and interspersed links. In any real
  # hypertext app, this method would parse a file to identify the links.
  def show_page(page)
    @buffer.text = ""
    case page
    when 1
      generate_page_1
    when 2
      generate_page_2
    when 3
      generate_page_3
    end
  end

  def generate_page_1
    iter = @buffer.get_iter_at(:offset => 0)
    @buffer.insert(iter, "Some text to show that simple ")
    insert_link(iter, "hyper text", 3)
    @buffer.insert(iter, " can easily be realized with ")
    insert_link(iter, "tags", 2)
    @buffer.insert(iter, ".")
  end

  def generate_page_2
    iter = @buffer.get_iter_at(:offset => 0)
    @buffer.insert(iter, <<-EOF)
A tag is an attribute that can be applied to some range of text.
For example, a tag might be called "bold" and make the text inside
the tag bold. However, the tag concept is more general than that;
tags don't have to affect appearance. They can instead affect the
behavior of mouse and key presses, "lock" a range of text so the
user can't edit it, or countless other things.\n
EOF
    insert_link(iter, "Go back", 1)
  end

  def generate_page_3
    iter = @buffer.get_iter_at(:offset => 0)
    tag = @buffer.create_tag(nil,
                             "weight" => :bold)
    @buffer.insert(iter, "hypertext:\n", :tags => [tag])
    @buffer.insert(iter, <<-EOF)
machine-readable text that is not sequential but is organized
so that related items of information are connected
EOF
    insert_link(iter, "Go back", 1)
  end

  def insert_link(iter, text, page)
    tag = @buffer.create_tag(nil,
                             "foreground" => "blue",
                             "underline" => :single)
    tag.page = page
    @buffer.insert(iter, text, :tags => [tag])
  end

  # Links can also be activated by clicking or tapping.
  def event_after(event)
    case event.type
    when Gdk::EventType::BUTTON_RELEASE
      return false unless event.button == Gdk::BUTTON_PRIMARY
    when Gdk::EventType::TOUCH_END
    else
      return false
    end

    buffer = @view.buffer

    # we shouldn't follow a link if the user has selected something
    range = buffer.selection_bounds
    return false if range && range[0].offset != range[1].offset

    x, y = @view.window_to_buffer_coords(:widget, event.x, event.y)
    iter = @view.get_iter_at_location(x, y)
    follow_if_link(iter) if iter
    true
  end

  # Looks at all tags covering the position of iter in the text view,
  # and if one of them is a link, follow it by showing the page identified
  # by the data attached to it.
  def follow_if_link(iter)
    tags = iter.tags
    tags.each do |tag|
      if tag.page
        show_page(tag.page)
        break
      end
    end
  end

  # Looks at all tags covering the position (x, y) in the text view,
  # and if one of them is a link, change the cursor to the "hands" cursor
  # typically used by web browsers.
  def set_cursor_if_appropriate(text_view, x, y)
    iter = text_view.get_iter_at_location(x, y)
    return unless iter
    hovering = false
    tags = iter.tags
    tags.each do |tag|
      if tag.page
        hovering = true
        break
      end
    end

    if hovering != @hovering
      @hovering = hovering
      window = text_view.get_window(:text)
      window.cursor = @hovering ? @hand_cursor : @regular_cursor
    end
  end
end

module Gtk
  class TextTag
    attr_accessor :page
  end
end
