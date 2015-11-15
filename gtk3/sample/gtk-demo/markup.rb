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

    window.set_title("Markup")

    view = Gtk::TextView.new
    view.set_wrap_mode(:word)
    view.set_left_margin(10)
    view.set_right_margin(10)

    sw = Gtk::ScrolledWindow.new(nil, nil)
    sw.set_policy(:never, :automatic)

    window.add(sw)
    sw.add(view)

    markup = Gio::Resources.lookup_data("/markup/markup.txt", 0)
    
    buffer = view.buffer
    buffer.insert_markup(buffer.start_iter, markup, -1)

    sw.show_all

    if !window.visible?
      window.show_all
    else
      window.destroy
    end
  end
end
