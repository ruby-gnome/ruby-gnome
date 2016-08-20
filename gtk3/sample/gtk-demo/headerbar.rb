# Copyright (c) 2015-2016 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
= Header Bar

GtkHeaderBar is a container that is suitable for implementing
window titlebars. One of its features is that it can position
a title (and optional subtitle) centered with regard to the
full width, regardless of variable-width content at the left
or right.

It is commonly used with gtk_window_set_titlebar()
=end
class HeaderbarDemo
  def initialize(main_window)
    @window = Gtk::Window.new(:toplevel)
    @window.screen = main_window.screen
    @window.set_default_size(600, 400)

    header = Gtk::HeaderBar.new
    header.show_close_button = true
    header.title = "Welcome to Facebook - Log in, sign up or learn more"
    header.has_subtitle = false

    button = Gtk::Button.new

    icon = Gio::ThemedIcon.new("mail-send-receive-symbolic")
    image = Gtk::Image.new(:icon => icon, :size => :button)

    button.add(image)
    header.pack_end(button)

    box = Gtk::Box.new(:horizontal, 0)
    box.style_context.add_class("linked")

    button = Gtk::Button.new
    image = Gtk::Image.new(:icon_name => "pan-start-symbolic", :size => :button)
    button.add(image)
    box.add(button)

    button = Gtk::Button.new
    image = Gtk::Image.new(:icon_name => "pan-end-symbolic", :size => :button)
    button.add(image)
    box.add(button)

    header.pack_start(box)
    @window.titlebar = header
    @window.add(Gtk::TextView.new)
  end

  def run
    if !@window.visible?
      @window.show_all
    else
      @window.destroy
    end
  end
end
