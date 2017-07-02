# Copyright (c) 2015-2016 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
= Text View/Markup

Gtk::TextBuffer lets you define your own tags that can influence
text formatting in a variety of ways. In this example, we show
that Gtk::TextBuffer can load Pango markup and automatically generate
suitable tags.

=end
class MarkupDemo
  def initialize(main_window)
    @window = Gtk::Window.new(:toplevel)
    @window.screen = main_window.screen
    @window.title = "Markup"
    @window.set_default_size(450, 450)

    initialize_stack
    initialize_show_source
    initialize_headerbar

    sw = generate_sourceview
    @view = sw.child
    @stack.add_named(sw, "formatted")

    sw = generate_sourceview
    @view2 = sw.child
    @stack.add_named(sw, "source")

    populate_sourceviews
  end

  def run
    if !@window.visible?
      @window.show_all
    else
      @window.destroy
    end
  end

  private

  def initialize_stack
    @stack = Gtk::Stack.new
    @stack.show
    @window.add(@stack)
  end

  def initialize_headerbar
    header = Gtk::HeaderBar.new
    header.pack_start(@show_source)
    header.show_close_button = true
    header.show_all
    @window.titlebar = header
  end

  def show_source_toggled_signal
    @show_source.signal_connect "toggled" do |button|
      if button.active?
        @stack.visible_child_name = "source"
      else
        buffer = @view2.buffer
        markup = buffer.get_text(buffer.start_iter, buffer.end_iter, false)
        buffer = @view.buffer
        start_iter, start_end = buffer.bounds
        buffer.delete(start_iter, start_end)
        buffer.insert_markup(buffer.start_iter, markup)
        @stack.visible_child_name = "formatted"
      end
    end
  end

  def initialize_show_source
    @show_source = Gtk::CheckButton.new("Source")
    @show_source.valign = :center
    show_source_toggled_signal
  end

  def generate_sourceview
    view = Gtk::TextView.new
    view.wrap_mode = :word
    view.left_margin = 10
    view.right_margin = 10

    sw = Gtk::ScrolledWindow.new(nil, nil)
    sw.set_policy(:automatic, :automatic)

    sw.add(view)
    sw.show_all
    sw
  end

  def populate_sourceviews
    markup = Gio::Resources.lookup_data("/markup/markup.txt", 0)

    buffer = @view.buffer
    buffer.insert_markup(buffer.start_iter, markup)

    buffer = @view2.buffer
    buffer.insert(buffer.start_iter, markup)

    @stack.show
  end
end
