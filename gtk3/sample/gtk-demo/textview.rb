# Copyright (c) 2017 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
=  Text View/Multiple Views

 The GtkTextView widget displays a GtkTextBuffer. One GtkTextBuffer
 can be displayed by multiple GtkTextViews. This demo has two views
 displaying a single buffer, and shows off the widget's text
 formatting features.

=end
class TextviewDemo
  def initialize(main_window)
    @window = Gtk::Window.new(:toplevel)
    @window.screen = main_window.screen
    @window.set_default_size(450, 450)
    @window.title = "Multiple Views"

    vpaned = Gtk::Paned.new(:vertical)
    @window.add(vpaned)

    # For convenience, we just use the autocreated buffer from
    # the first text view; you could also create the buffer
    # by itself Gtk::TextBuffer.new, then later create a view
    # widget.

    view1 = Gtk::TextView.new
    view2 = Gtk::TextView.new(view1.buffer)

    sw = Gtk::ScrolledWindow.new
    sw.set_policy(:automatic, :automatic)
    vpaned.add1(sw)
    sw.add(view1)

    sw = Gtk::ScrolledWindow.new
    sw.set_policy(:automatic, :automatic)
    vpaned.add2(sw)
    sw.add(view2)

    create_tags(view1.buffer)
    insert_text(view1.buffer)

    attach_widgets(view1)
    attach_widgets(view2)

    vpaned.show_all
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

  def create_tags(buffer)
    # Create a bunch of tags. Note that it's also possible to
    # create tags with Gtk::TextTag.new then add them to the
    # tag table for the buffer, Gtk::TextBuffer.create_tag is
    # just a convenience function. Also note that you don't have
    # to give tags a name; pass NULL for the name to create an
    # anonymous tag.
    #
    # In any real app, another useful optimization would be to create
    # a Gtk::TextTagTable in advance, and reuse the same tag table for
    # all the buffers with the same tag set, instead of creating
    # new copies of the same tags for every buffer.
    #
    # Tags are assigned default priorities in order of addition to the
    # tag table.  That is, tags created later that affect the same text
    # property affected by an earlier tag will override the earlier
    # tag.  You can modify tag priorities with
    # Gtk::TextTag.priority=.

    buffer.create_tag("heading",
                      "weight" => :bold,
                      "size" => 15 * Pango::SCALE)

    buffer.create_tag("italic",
                      "style" => :italic)

    buffer.create_tag("bold",
                      "weight" => :bold)

    buffer.create_tag("big",
                      # points times the PANGO_SCALE factor
                      "size" => 20 * Pango::SCALE)

    buffer.create_tag("xx-small",
                      "scale" => :xx_small)

    buffer.create_tag("x-large",
                      "scale" => :x_large)

    buffer.create_tag("monospace", "family" => "monospace")

    buffer.create_tag("blue_foreground", "foreground" => "blue")

    buffer.create_tag("red_background", "background" => "red")

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
                      "wrap_mode" => :char)

    buffer.create_tag("no_wrap",
                      "wrap_mode" => :none)

    buffer.create_tag("center",
                      "justification" => :center)

    buffer.create_tag("right_justify",
                      "justification" => :right)

    buffer.create_tag("wide_margins",
                      "left_margin" => 50,
                      "right_margin" => 50)

    buffer.create_tag("strikethrough",
                      "strikethrough" => true)

    buffer.create_tag("underline",
                      "underline" => :single)

    buffer.create_tag("double_underline",
                      "underline" => :double)

    buffer.create_tag("superscript",
                      "rise" => 10 * Pango::SCALE, #  10 pixels
                      "size" => 8 * Pango::SCALE)  #  8 points

    buffer.create_tag("subscript",
                      "rise" => -10 * Pango::SCALE, #  10 pixels
                      "size" => 8 * Pango::SCALE) #  8 points

    buffer.create_tag("rtl_quote",
                      "wrap_mode" => :word,
                      "direction" => :rtl,
                      "indent" => 30,
                      "left_margin" => 20,
                      "right_margin" => 20)
  end

  def insert_text(buffer)
    icon_theme = Gtk::IconTheme.default
    pixbuf = icon_theme.load_icon("gtk3-demo", 32, :generic_fallback)

    # get start of buffer; each insertion will revalidate the
    # iterator to point to just after the inserted text.

    iter = buffer.get_iter_at(:offset => 0)
    buffer.insert(iter, <<TEXT)
The text widget can display text with all kinds of nifty attributes. It \
also supports multiple views of the same buffer; this demo is showing the \
same buffer in two places.
TEXT
    buffer.insert(iter, "Font styles. ", :tags => ["heading"])
    buffer.insert(iter, "For example, you can have ")
    buffer.insert(iter, "italic", :tags => ["italic"])
    buffer.insert(iter, ", ")
    buffer.insert(iter, "bold", :tags => ["bold"])
    buffer.insert(iter, ", or ")
    buffer.insert(iter, "monospace(typewriter)", :tags => ["monospace"])
    buffer.insert(iter, ", or ")
    buffer.insert(iter, "big", :tags => ["big"])
    buffer.insert(iter, " text. ")
    buffer.insert(iter, "It's best not to hardcode specific text sizes; you can\
 use relative sizes as with CSS, such as ")
    buffer.insert(iter, "xx-small", :tags => ["xx-small"])
    buffer.insert(iter, " or ")
    buffer.insert(iter, "x-large", :tags => ["x-large"])
    buffer.insert(iter, " to ensure that your program properly adapts if the \
user changes the default font size.\n\n")

    buffer.insert(iter, "Colors. ", :tags => ["heading"])

    buffer.insert(iter, "Colors such as ")
    buffer.insert(iter, "a blue foreground", :tags => ["blue_foreground"])
    buffer.insert(iter, " or ")
    buffer.insert(iter, "a red background", :tags => ["red_background"])
    buffer.insert(iter, " or even ")
    buffer.insert(iter, "a blue foreground on red background",
                  :tags => %w(blue_foreground red_background))

    buffer.insert(iter, "(select that to read it) can be used.\n\n")

    buffer.insert(iter, "Underline, strikethrough, and rise. ",
                  :tags => ["heading"])

    buffer.insert(iter, "Strikethrough", :tags => ["strikethrough"])
    buffer.insert(iter, ", ")
    buffer.insert(iter, "underline", :tags => ["underline"])
    buffer.insert(iter, ", ")
    buffer.insert(iter, "double underline", :tags => ["double_underline"])
    buffer.insert(iter, ", ")
    buffer.insert(iter, "superscript", :tags => ["superscript"])
    buffer.insert(iter, ", and ")
    buffer.insert(iter, "subscript", :tags => ["subscript"])
    buffer.insert(iter, " are all supported.\n\n")

    buffer.insert(iter, "Images. ", :tags => ["heading"])

    buffer.insert(iter, "The buffer can have images in it: ")
    buffer.insert(iter, pixbuf)
    buffer.insert(iter, pixbuf)
    buffer.insert(iter, pixbuf)
    buffer.insert(iter, " for example.\n\n")

    buffer.insert(iter, "Spacing. ", :tags => ["heading"])

    buffer.insert(iter,
                  "You can adjust the amount of space before each line.\n")

    buffer.insert(iter, "This line has a whole lot of space before it.\n",
                  :tags => %w(big_gap_before_line wide_margins))
    buffer.insert(iter, "You can also adjust the amount of space after each \
line; this line has a whole lot of space after it.\n",
                  :tags => %w(big_gap_after_line wide_margins))
    text = "You can also adjust the amount of space between wrapped lines; \
this line has extra space between each wrapped line in the same paragraph. To \
show off wrapping, some filler text: the quick brown fox jumped over the lazy \
dog. Blah blah blah blah blah blah blah blah blah.\n"
    buffer.insert(iter, text,
                  :tags => %w(double_spaced_line wide_margins))

    buffer.insert(iter,
                  "Also note that those lines have extra-wide margins.\n\n")

    buffer.insert(iter, "Editability. ", :tags => ["heading"])

    buffer.insert(iter, "This line is 'locked down' and can't be edited by the \
user - just try it! You can't delete this line.\n\n",
                  :tags => ["not_editable"])

    buffer.insert(iter, "Wrapping. ", :tags => ["heading"])

    buffer.insert(iter, "This line(and most of the others in this buffer) is \
word-wrapped, using the proper Unicode algorithm. Word wrap should work in all\
 scripts and languages that GTK+ supports. Let's make this a long paragraph to\
 demonstrate: blah blah blah blah blah blah blah blah blah blah blah blah blah\
 blah blah blah blah blah blah\n\n")

    buffer.insert(iter, "This line has character-based wrapping, and can wrap \
between any two character glyphs. Let's make this a long paragraph to \
demonstrate: blah blah blah blah blah blah blah blah blah blah blah blah blah \
blah blah blah blah blah blah\n\n", :tags => ["char_wrap"])

    buffer.insert(iter, "This line has all wrapping turned off, so it makes \
the horizontal scrollbar appear.\n\n\n", :tags => ["no_wrap"])

    buffer.insert(iter, "Justification. ", :tags => ["heading"])

    buffer.insert(iter, "\nThis line has center justification.\n",
                  :tags => ["center"])

    buffer.insert(iter, "This line has right justification.\n",
                  :tags => ["right_justify"])

    buffer.insert(iter, "\nThis line has big wide margins. Text text text text\
 text text text text text text text text text text text text text text text \
text text text text text text text text text text text text text text text \
text text.\n", :tags => ["wide_margins"])

    buffer.insert(iter, "Internationalization. ", :tags => ["heading"])

    buffer.insert(iter, "You can put all sorts of Unicode text in the buffer.\
\n\nGerman(Deutsch Süd) Grüß Gott\nGreek(Ελληνικά) Γειά σας\nHebrew        \
שלום\nJapanese(日本語)\n\nThe widget properly handles bidirectional text, word\
 wrapping, DOS/UNIX/Unicode paragraph separators, grapheme boundaries, and so \
 on using the Pango internationalization framework.\n")

    buffer.insert(iter,
                  "Here's a word-wrapped quote in a right-to-left language:\n")
    buffer.insert(iter, "\331\210\331\202\330\257 \330\250\330\257\330\243 \
\330\253\331\204\330\247\330\253 \331\205\331\206 \330\243\331\203\330\253\330\
\261 \330\247\331\204\331\205\330\244\330\263\330\263\330\247\330\252 \330\252\
\331\202\330\257\331\205\330\247 \331\201\331\212 \330\264\330\250\331\203\330\
\251 \330\247\331\203\330\263\331\212\331\210\331\206 \330\250\330\261\330\247\
\331\205\330\254\331\207\330\247 \331\203\331\205\331\206\330\270\331\205\330\
\247\330\252 \331\204\330\247 \330\252\330\263\330\271\331\211 \331\204\331\
\204\330\261\330\250\330\255\330\214 \330\253\331\205 \330\252\330\255\331\210\
\331\204\330\252 \331\201\331\212 \330\247\331\204\330\263\331\206\331\210\330\
\247\330\252 \330\247\331\204\330\256\331\205\330\263 \330\247\331\204\331\205\
\330\247\330\266\331\212\330\251 \330\245\331\204\331\211 \331\205\330\244\330\
\263\330\263\330\247\330\252 \331\205\330\247\331\204\331\212\330\251 \331\205\
\331\206\330\270\331\205\330\251\330\214 \331\210\330\250\330\247\330\252\330\
\252 \330\254\330\262\330\241\330\247 \331\205\331\206 \330\247\331\204\331\
\206\330\270\330\247\331\205 \330\247\331\204\331\205\330\247\331\204\331\212 \
\331\201\331\212 \330\250\331\204\330\257\330\247\331\206\331\207\330\247\330\
\214 \331\210\331\204\331\203\331\206\331\207\330\247 \330\252\330\252\330\256\
\330\265\330\265 \331\201\331\212 \330\256\330\257\331\205\330\251 \331\202\
\330\267\330\247\330\271 \330\247\331\204\331\205\330\264\330\261\331\210\330\
\271\330\247\330\252 \330\247\331\204\330\265\330\272\331\212\330\261\330\251.\
\331\210\330\243\330\255\330\257 \330\243\331\203\330\253\330\261 \331\207\330\
\260\331\207 \330\247\331\204\331\205\330\244\330\263\330\263\330\247\330\252 \
\331\206\330\254\330\247\330\255\330\247 \331\207\331\210 \302\273\330\250\330\
\247\331\206\331\203\331\210\330\263\331\210\331\204\302\253 \331\201\331\212 \
\330\250\331\210\331\204\331\212\331\201\331\212\330\247.\n\n" ,
                  :tags => ["rtl_quote"])

    buffer.insert(iter, "You can put widgets in the buffer: Here's a button: ")
    buffer.create_child_anchor(iter)
    buffer.insert(iter, " and a menu: ")
    buffer.create_child_anchor(iter)
    buffer.insert(iter, " and a scale: ")
    buffer.create_child_anchor(iter)
    buffer.insert(iter, " and an animation: ")
    buffer.create_child_anchor(iter)
    buffer.insert(iter, " finally a text entry: ")
    buffer.create_child_anchor(iter)
    buffer.insert(iter, ".\n")

    buffer.insert(iter, "\n\nThis demo doesn't demonstrate all the \
Gtk::TextBuffer features; it leaves out, for example: invisible/hidden text \
(doesn't work in GTK 2, but planned), tab stops, application-drawn areas on \
the sides of the widget for displaying breakpoints and such...")

    #  Apply word_wrap tag to whole buffer

    buf_start, buf_end = buffer.bounds
    buffer.apply_tag("word_wrap", buf_start, buf_end)
  end

  def find_anchor(iter)
    while iter.forward_char
      return true if iter.child_anchor
    end
    false
  end

  def attach_widgets(text_view)
    buffer = text_view.buffer
    iter = buffer.start_iter
    i = 0
    widget = nil
    while find_anchor(iter)
      anchor = iter.child_anchor

      if i == 0
        widget = Gtk::Button.new(:label => "Click Me")
        widget.signal_connect("clicked") { easter_egg_callback }
      elsif i == 1
        widget = Gtk::ComboBoxText.new
        widget.append_text("Option 1")
        widget.append_text("Option 2")
        widget.append_text("Option 3")
      elsif i == 2
        widget = Gtk::Scale.new(:horizontal, nil)
        widget.set_range(0, 100)
        widget.set_size_request(70, -1)
      elsif i == 3
        widget = Gtk::Image.new(:resource => "/textview/floppybuddy.gif")
      elsif i == 4
        widget = Gtk::Entry.new
      end
      text_view.add_child_at_anchor(widget, anchor)
      widget.show_all
      i += 1
    end
  end

  def easter_egg_callback
    buffer = Gtk::TextBuffer.new
    iter = buffer.start_iter
    buffer.insert(iter,
                  "This buffer is shared by a set of nested text views.\n \
 Nested view:")
    anchor = buffer.create_child_anchor(iter)
    buffer.insert(iter,
                  "\nDon't do this in real applications, please.\n")
    view = Gtk::TextView.new(buffer)
    recursive_attach_view(0, view, anchor)

    window = Gtk::Window.new
    sw = Gtk::ScrolledWindow.new
    sw.set_policy(:automatic, :automatic)
    window.add(sw)
    sw.add(view)
    window.set_default_size(300, 400)
    window.show_all
  end

  def recursive_attach_view(depth, view, anchor)
    return if depth > 4

    child_view = Gtk::TextView.new(view.buffer)

    # Frame is to add a black border around each child view.
    frame = Gtk::Frame.new
    frame.add(child_view)
    view.add_child_at_anchor(frame, anchor)
    recursive_attach_view(depth + 1, child_view, anchor)
  end
end
