# Copyright (c) 2015 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
= Text View/Markup

Gtk::TextBuffer lets you define your own tags that can influence
text formatting in a variety of ways. In this example, we show
that Gtk::TextBuffer can load Pango markup and automatically generate
suitable tags.

=end
module MarkupDemo

  def self.run_demo(main_window)
    window = Gtk::Window.new(:toplevel)
    window.screen = main_window.screen
    window.set_default_size(450, 450)

    stack = Gtk::Stack.new
    stack.show
    window.add(stack)

    show_source = Gtk::CheckButton.new("Source")
    show_source.valign = :center

    header = Gtk::HeaderBar.new
    header.pack_start(show_source)
    header.show_close_button = true
    header.show_all
    window.titlebar = header

    window.title = "Markup"

    view = Gtk::TextView.new
    view.wrap_mode = :word
    view.left_margin = 10
    view.right_margin = 10

    sw = Gtk::ScrolledWindow.new(nil, nil)
    sw.set_policy(:automatic, :automatic)

    sw.add(view)
    sw.show_all

    stack.add_named(sw, "formatted")

    view2 = Gtk::TextView.new
    view2.wrap_mode = :word
    view2.left_margin = 10
    view2.right_margin = 10

    sw = Gtk::ScrolledWindow.new(nil, nil)
    sw.set_policy(:automatic, :automatic)

    sw.add(view2)
    sw.show_all

    stack.add_named(sw, "source")

    markup = Gio::Resources.lookup_data("/markup/markup.txt", 0)

    show_source.signal_connect "toggled" do |button|
      if button.active?
        stack.visible_child_name = "source"
      else
        buffer = view2.buffer
        markup = buffer.get_text(buffer.start_iter,
                                 buffer.end_iter,
                                 false)
        buffer = view.buffer
        buffer.insert_markup(buffer.start_iter, markup, -1)
        stack.visible_child_name = "formatted"
      end
    end

    buffer = view.buffer
    buffer.insert_markup(buffer.start_iter, markup, -1)

    buffer = view2.buffer
    buffer.insert(buffer.start_iter, markup)

    stack.show

    if !window.visible?
      window.show_all
    else
      window.destroy
    end
  end
end
