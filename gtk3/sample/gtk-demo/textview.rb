# -*- coding: utf-8 -*-
# Copyright (c) 2003-2005 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
# $Id: textview.rb,v 1.8 2005/02/06 18:25:13 kzys Exp $
=begin
= Text Widget/Multiple Views

The Gtk::TextView widget displays a Gtk::TextBuffer.
One Gtk::TextBuffer can be displayed by multiple Gtk::TextViews.
This demo has two views displaying a single buffer,
and shows off the widget's text formatting features.
=end
require 'common'

module Demo
  class TextView < BasicWindow

    GRAY50_WIDTH = 2
    GRAY50_HEIGHT = 2
    GRAY50_BITS = [0x02, 0x01].pack("c")
    @@nest_window = nil

    def initialize
      super("TextView Window")

      set_default_size(450, 450)

      set_title("TextView")
      set_border_width(0)

      vpaned = Gtk::VPaned.new
      vpaned.set_border_width(5)
      add(vpaned)
      ##
      # For convenience, we just use the autocreated buffer from
      # the first text view; you could also create the buffer
      # by itself with Gtk::TextBuffer.new, then later create
      # a view widget.
      #
      view1 = Gtk::TextView.new
      buffer = view1.buffer
      view2 = Gtk::TextView.new(buffer)

      sw = Gtk::ScrolledWindow.new(nil, nil)
      sw.set_policy(:automatic,
                    :automatic)
      vpaned.add1(sw)

      sw.add(view1)

      sw = Gtk::ScrolledWindow.new(nil, nil)
      sw.set_policy(:automatic,
                    :automatic)
      vpaned.add2(sw)

      sw.add(view2)

      create_tags(buffer)
      insert_text(buffer)

      attach_widgets(view1)
      attach_widgets(view2)

      vpaned.show_all

      self.show
    end

    def create_tags(buffer)
      ##
      #  Create a bunch of tags. Note that it's also possible to
      #  create tags with Gtk::TextTag.new then add them to the
      #  tag table for the buffer, Gtk::TextBuffer#create_tag is
      #  just a convenience function. Also note that you don't have
      #  to give tags a name; pass NULL for the name to create an
      #  anonymous tag.
      #
      #  In any real app, another useful optimization would be to create
      #  a Gtk::TextTagTable in advance, and reuse the same tag table for
      #  all the buffers with the same tag set, instead of creating
      #  new copies of the same tags for every buffer.
      #
      #  Tags are assigned default priorities in order of addition to the
      #  tag table.         That is, tags created later that affect the same text
      #  property affected by an earlier tag will override the earlier
      #  tag.  You can modify tag priorities with
      #  Gtk::TextTag#set_priority.
      #

      buffer.create_tag('heading',
                        'weight' => Pango::FontDescription::WEIGHT_BOLD,
                        'size' => 15 * Pango::SCALE)

      buffer.create_tag("italic",
                        "style" => Pango::FontDescription::STYLE_ITALIC)

      buffer.create_tag("bold",
                        "weight" => Pango::FontDescription::WEIGHT_BOLD)

      buffer.create_tag("big",
                        # points times the PANGO_SCALE factor
                        "size" => 20 * Pango::SCALE)

      buffer.create_tag("xx-small",
                        "scale" => Pango::AttrScale::XX_SMALL)

      buffer.create_tag("x-large",
                        "scale" => Pango::AttrScale::X_LARGE)

      buffer.create_tag("monospace", "family" => "monospace")

      buffer.create_tag("blue_foreground", "foreground" => "blue")

      buffer.create_tag("red_background", "background" => "red")

      stipple = Gdk::Pixmap.create_from_data(nil, GRAY50_BITS, GRAY50_WIDTH,
                                             GRAY50_HEIGHT)

      buffer.create_tag("background_stipple",
                        "background_stipple" => stipple)

      buffer.create_tag("foreground_stipple",
                        "foreground_stipple" => stipple)

      buffer.create_tag("big_gap_before_line",
                        "pixels_above_lines" => 30)

      buffer.create_tag("big_gap_after_line",
                        "pixels_below_lines" => 30)

      buffer.create_tag("double_spaced_line",
                        "pixels_inside_wrap" => 10)

      buffer.create_tag("not_editable",
                         "editable" => false)

      buffer.create_tag("word_wrap",
                        "wrap_mode" => :word)

      buffer.create_tag("char_wrap",
                        "wrap_mode" => Gtk::TextTag::WRAP_CHAR)

      buffer.create_tag("no_wrap",
                        "wrap_mode" => :none)

      buffer.create_tag("center",
                        "justification" => Gtk::JUSTIFY_CENTER)

      buffer.create_tag("right_justify",
                        "justification" => Gtk::JUSTIFY_RIGHT)

      buffer.create_tag("wide_margins",
                        "left_margin" => 50,
                          "right_margin" => 50)

      buffer.create_tag("strikethrough",
                        "strikethrough" => true)

      buffer.create_tag("underline",
                        "underline" => Pango::AttrUnderline::SINGLE)

      buffer.create_tag("double_underline",
                        "underline" => Pango::AttrUnderline::DOUBLE)

      buffer.create_tag("superscript",
                        "rise" => 10 * Pango::SCALE, #  10 pixels
                        "size" => 8 * Pango::SCALE)  #  8 points

      buffer.create_tag("subscript",
                        "rise" => -10 * Pango::SCALE, #  10 pixels
                        "size" => 8 * Pango::SCALE) #  8 points

      buffer.create_tag("rtl_quote",
                        "wrap_mode" => :word,
                        "direction" => Gtk::Widget::TEXT_DIR_RTL,
                        "indent" => 30,
                        "left_margin" => 20,
                        "right_margin" => 20)
    end

    def insert_text(buffer)

      ##
      #  Demo.find_file looks in the the current directory first,
      # so you can run gtk-demo without installing GTK, then looks
      # in the location where the file is installed.
      #

      filename = Demo.find_file("gtk-logo-rgb.gif")
      pixbuf = Gdk::Pixbuf.new(filename) if filename

      unless pixbuf
        $stderr.print "Failed to load image file gtk-logo-rgb.gif\n"
        exit 1
      end

      scaled = pixbuf.scale(32, 32, Gdk::Pixbuf::INTERP_BILINEAR)
      pixbuf = scaled

      ##
      #  get start of buffer; each insertion will revalidate the
      #  iterator to point to just after the inserted text.
      #

      iter = buffer.get_iter_at(:offset => 0)

      buffer.insert(iter, "The text widget can display text with all kinds of nifty attributes. It also supports multiple views of the same buffer; this demo is showing the same buffer in two places.\n\n")

      buffer.insert(iter, "Font styles. ", 'heading')

      buffer.insert(iter, "For example, you can have ")
      buffer.insert(iter, "italic", "italic")
      buffer.insert(iter, ", ")
      buffer.insert(iter, "bold", "bold")
      buffer.insert(iter, ", or ")
      buffer.insert(iter, "monospace(typewriter)", "monospace")
      buffer.insert(iter, ", or ")
      buffer.insert(iter, "big", "big")
      buffer.insert(iter, " text. ")
      buffer.insert(iter, "It's best not to hardcode specific text sizes; you can use relative sizes as with CSS, such as ")
      buffer.insert(iter, "xx-small", "xx-small")
      buffer.insert(iter, " or ")
      buffer.insert(iter, "x-large", "x-large")
      buffer.insert(iter, " to ensure that your program properly adapts if the user changes the default font size.\n\n")

      buffer.insert(iter, "Colors. ", "heading")

      buffer.insert(iter, "Colors such as ")
      buffer.insert(iter, "a blue foreground", "blue_foreground")
      buffer.insert(iter, " or ")
      buffer.insert(iter, "a red background", "red_background")
      buffer.insert(iter, " or even ")
      buffer.insert(iter, "a stippled red background",
                    "red_background", "background_stipple")

      buffer.insert(iter, " or ")
      buffer.insert(iter, "a stippled blue foreground on solid red background",
                    "blue_foreground", "red_background", "foreground_stipple")

      buffer.insert(iter, "(select that to read it) can be used.\n\n")

      buffer.insert(iter, "Underline, strikethrough, and rise. ", "heading")

      buffer.insert(iter, "Strikethrough", "strikethrough")
      buffer.insert(iter, ", ")
      buffer.insert(iter, "underline", "underline")
      buffer.insert(iter, ", ")
      buffer.insert(iter, "double underline", "double_underline")
      buffer.insert(iter, ", ")
      buffer.insert(iter, "superscript", "superscript")
      buffer.insert(iter, ", and ")
      buffer.insert(iter, "subscript", "subscript")
      buffer.insert(iter, " are all supported.\n\n")

      buffer.insert(iter, "Images. ", "heading")

      buffer.insert(iter, "The buffer can have images in it: ")
      buffer.insert(iter, pixbuf)
      buffer.insert(iter, pixbuf)
      buffer.insert(iter, pixbuf)
      buffer.insert(iter, " for example.\n\n")

      buffer.insert(iter, "Spacing. ", "heading")

      buffer.insert(iter, "You can adjust the amount of space before each line.\n")

      buffer.insert(iter, "This line has a whole lot of space before it.\n",
                    "big_gap_before_line", "wide_margins")
      buffer.insert(iter, "You can also adjust the amount of space after each line; this line has a whole lot of space after it.\n",
                    "big_gap_after_line", "wide_margins")

      buffer.insert(iter, "You can also adjust the amount of space between wrapped lines; this line has extra space between each wrapped line in the same paragraph. To show off wrapping, some filler text: the quick brown fox jumped over the lazy dog. Blah blah blah blah blah blah blah blah blah.\n",
                    "double_spaced_line", "wide_margins")

      buffer.insert(iter, "Also note that those lines have extra-wide margins.\n\n")

      buffer.insert(iter, "Editability. ", "heading")

      buffer.insert(iter, "This line is 'locked down' and can't be edited by the user - just try it! You can't delete this line.\n\n",
                    "not_editable")

      buffer.insert(iter, "Wrapping. ", "heading")

      buffer.insert(iter, "This line(and most of the others in this buffer) is word-wrapped, using the proper Unicode algorithm. Word wrap should work in all scripts and languages that GTK+ supports. Let's make this a long paragraph to demonstrate: blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah\n\n")

      buffer.insert(iter, "This line has character-based wrapping, and can wrap between any two character glyphs. Let's make this a long paragraph to demonstrate: blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah\n\n", "char_wrap")

      buffer.insert(iter, "This line has all wrapping turned off, so it makes the horizontal scrollbar appear.\n\n\n", "no_wrap")

      buffer.insert(iter, "Justification. ", "heading")

      buffer.insert(iter, "\nThis line has center justification.\n", "center")

      buffer.insert(iter, "This line has right justification.\n",
                    "right_justify")

      buffer.insert(iter, "\nThis line has big wide margins. Text text text text text text text text text text text text text text text text text text text text text text text text text text text text text text text text text text text text.\n", "wide_margins")

      buffer.insert(iter, "Internationalization. ", "heading")

      buffer.insert(iter, "You can put all sorts of Unicode text in the buffer.\n\nGerman(Deutsch Süd) Grüß Gott\nGreek(Ελληνικά) Γειά σας\nHebrew        שלום\nJapanese(日本語)\n\nThe widget properly handles bidirectional text, word wrapping, DOS/UNIX/Unicode paragraph separators, grapheme boundaries, and so on using the Pango internationalization framework.\n")

      buffer.insert(iter, "Here's a word-wrapped quote in a right-to-left language:\n")
      buffer.insert(iter, "??? ??? ???? ?? ???? ???????? ????? ?? ???? ?????? ??????? ??????? ?? ???? ?????? ?? ????? ?? ??????? ????? ??????? ??? ?????? ????? ?????? ????? ???? ?? ?????? ?????? ?? ???????? ?????? ????? ?? ???? ???? ????????? ???????. ???? ???? ??? ???????? ????? ?? »????????« ?? ???????.\n\n", "rtl_quote")

      buffer.insert(iter, "You can put widgets in the buffer: Here's a button: ")
      anchor = buffer.create_child_anchor(iter)
      buffer.insert(iter, " and a menu: ")
      anchor = buffer.create_child_anchor(iter)
      buffer.insert(iter, " and a scale: ")
      anchor = buffer.create_child_anchor(iter)
      buffer.insert(iter, " and an animation: ")
      anchor = buffer.create_child_anchor(iter)
      buffer.insert(iter, " finally a text entry: ")
      anchor = buffer.create_child_anchor(iter)
      buffer.insert(iter, ".\n")

      buffer.insert(iter, "\n\nThis demo doesn't demonstrate all the Gtk::TextBuffer features; it leaves out, for example: invisible/hidden text(doesn't work in GTK 2, but planned), tab stops, application-drawn areas on the sides of the widget for displaying breakpoints and such...")

      #  Apply word_wrap tag to whole buffer

      buf_start, buf_end = buffer.bounds
      buffer.apply_tag("word_wrap", buf_start, buf_end)
    end

    def find_anchor(iter)
      while iter.forward_char do
        return true if iter.child_anchor
      end
      return false
    end

    def attach_widgets(text_view)

      buffer = text_view.buffer;

      iter = buffer.start_iter

      i = 0;
      while find_anchor(iter) do
        anchor = iter.child_anchor

        if  i == 0
          widget = Gtk::Button.new("Click Me")
          widget.signal_connect('clicked') { |button|
            if @@nest_window and ! @@nest_window.destroyed?
              @@nest_window.present
              break
            end

            buffer = Gtk::TextBuffer.new
            iter = buffer.start_iter

            buffer.insert(iter, "This buffer is shared by a set of nested text views.\n Nested view:\n")
            anchor = buffer.create_child_anchor(iter)
            buffer.insert(iter, "\nDon't do this in real applications, please.\n")

            view = Gtk::TextView.new(buffer)

            recursive_attach_view(0, view, anchor)

            @@nest_window = Gtk::Window.new(Gtk::Window::TOPLEVEL)
            sw = Gtk::ScrolledWindow.new(nil, nil)
            sw.set_policy(:automatic,
                          :automatic)

            @@nest_window.add(sw)
            sw.add(view)

            @@nest_window.set_default_size(300, 400)
            @@nest_window.show_all
          }
        elsif i == 1
          menu = Gtk::Menu.new
          widget = Gtk::OptionMenu.new

          menu_item = Gtk::MenuItem.new("Option 1")
          menu.append(menu_item)
          menu_item = Gtk::MenuItem.new("Option 2")
          menu.append(menu_item)
          menu_item = Gtk::MenuItem.new("Option 3")
          menu.append(menu_item)

          widget.menu = menu

        elsif i == 2
          widget = Gtk::HScale.new
          widget.set_range(0, 100)
          widget.set_size_request(70, -1)

        elsif i == 3
          filename = Demo.find_file("floppybuddy.gif")
          widget = Gtk::Image.new(filename)

        elsif i == 4
          widget = Gtk::Entry.new

        else
          #widget = nil;  avoids a compiler warning
          #g_assert_not_reached ;
        end

        text_view.add_child_at_anchor(widget,
                                      anchor)

        widget.show_all
        i += 1
      end
    end


    def recursive_attach_view(depth, view, anchor)
      return  if depth > 4

      child_view = Gtk::TextView.new(view.buffer)

      #  Event box is to add a black border around each child view
      event_box = Gtk::EventBox.new
      color = Gdk::Color.parse("black")
      event_box.modify_bg(Gtk::STATE_NORMAL, color)

      align = Gtk::Alignment.new(0.5, 0.5, 1.0, 1.0)
      align.set_border_width(1)

      event_box.add(align)
      align.add(child_view)

      view.add_child_at_anchor(event_box, anchor)

      recursive_attach_view(depth + 1, child_view, anchor)
    end

  end
end

