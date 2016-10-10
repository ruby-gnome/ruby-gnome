# Copyright (c) 2016 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
=  Text View/Automatic Scrolling

 This example demonstrates how to use the gravity of
 GtkTextMarks to keep a text view scrolled to the bottom
 while appending text.
=end
class TextscrollDemo
  def initialize(main_window)
    @window = Gtk::Window.new(:toplevel)
    @window.screen = main_window.screen

    @window.set_default_size(600, 400)

    hbox = Gtk::Box.new(:horizontal, 6)
    hbox.homogeneous = true
    @window.add(hbox)

    create_text_view(hbox, true)
    create_text_view(hbox, false)
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

  def create_text_view(hbox, to_end)
    swindow = Gtk::ScrolledWindow.new
    hbox.pack_start(swindow, :fill => true, :expand => true, :padding => 0)
    textview = Gtk::TextView.new
    swindow.add(textview)
    timeout = setup_scroll(textview, to_end)
    textview.signal_connect("destroy") { GLib::Source.remove(timeout) }
  end

  def setup_scroll(textview, to_end)
    buffer = textview.buffer
    end_iter = buffer.end_iter
    count = 0
    if to_end
      # If we want to scroll to the end, including horizontal scrolling,
      # then we just create a mark with right gravity at the end of the
      # buffer. It will stay at the end unless explicitly moved with
      # gtk_text_buffer_move_mark.
      buffer.create_mark("end", end_iter, false)

      # add scrolling timeout
      return GLib::Timeout.add(50) do # scroll to end
        count += 1
        # Get "end" mark. It's located at the end of buffer because
        # of right gravity
        mark = buffer.get_mark("end")
        mark_iter = buffer.get_iter_at(:mark => mark)
        # and insert some text at its position, the iter will be
        # revalidated after insertion to point to the end of inserted text
        buffer.insert(mark_iter, "\n")
        buffer.insert(mark_iter, " " * count)
        buffer.insert(mark_iter, "Scroll to end scroll to end scroll" +
                                 " to end scroll to end #{count}")
        # Now scroll the end mark onscreen.
        textview.scroll_mark_onscreen(mark)

        # Emulate typewriter behavior, shift to the left if we
        # are far enough to the right.
        count = 0 if count > 150
        GLib::Source::CONTINUE
      end
    else
      # If we want to scroll to the bottom, but not scroll horizontally,
      # then an end mark won't do the job. Just create a mark so we can
      # use it with gtk_text_view_scroll_mark_onscreen, we'll position it
      # explicitly when needed. Use left gravity so the mark stays where
      # we put it after inserting new text.
      buffer.create_mark("scroll", end_iter, true)
      return GLib::Timeout.add(100) do # scroll to bottom
        count += 1
        # Get end iterator
        end_iter = buffer.end_iter
        # and insert some text at it, the iter will be revalidated
        # after insertion to point to the end of inserted text
        buffer.insert(end_iter, "\n")
        buffer.insert(end_iter, " " * count)
        buffer.insert(end_iter,"Scroll to bottom scroll to bottom scroll" +
                                  " to bottom scroll to bottom #{count}")
        # Move the iterator to the beginning of line, so we don't scroll
        # in horizontal direction
        end_iter.line_offset = 0
        # and place the mark at iter. the mark will stay there after we
        # insert some text at the end because it has left gravity.
        mark = buffer.get_mark("scroll")
        buffer.move_mark(mark, end_iter)
        # Scroll the mark onscreen.
        textview.scroll_mark_onscreen(mark)
        # Shift text back if we got enough to the right.
        count = 0 if count > 40
        GLib::Source::CONTINUE
      end
    end
  end
end
