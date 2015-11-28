# Copyright (c) 2015 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
= Color Chooser

A GtkColorChooser lets the user choose a color. There are several
implementations of the GtkColorChooser interface in GTK+. The
GtkColorChooserDialog is a prebuilt dialog containing a
GtkColorChooserWidget.
=end
module ColorselDemo
  def self.run_demo(main_window)
    color = Gdk::RGBA.new(0, 0, 1, 1)

    window = Gtk::Window.new(:toplevel)
    window.screen = main_window.screen
    window.set_title("Color Chooser")
    window.set_border_width(8)

    vbox = Gtk::Box.new(:vertical, 8)
    vbox.set_border_width(8)
    window.add(vbox)

    frame = Gtk::Frame.new
    frame.set_shadow_type(:in)
    vbox.pack_start(frame, :expand => true, :fill => true, :padding => 0)

    da = Gtk::DrawingArea.new
    da.signal_connect "draw" do |_widget, cr|
      cr.set_source(color.to_a)
      cr.paint
    end

    da.set_size_request(200, 200)

    frame.add(da)

    button = Gtk::Button.new(:mnemonic => "_Change the above color")
    button.set_halign(:end)
    button.set_valign(:center)

    vbox.pack_start(button, :expand => false, :fill => false, :padding => 0)
    button.signal_connect "clicked" do |_widget|
      dialog = Gtk::ColorChooserDialog.new(:title => "Changing Color",
                                           :parent => window)
      dialog.set_modal(true)
      dialog.set_rgba(color)

      dialog.signal_connect "response" do |widget, response_id|
        color = widget.rgba if response_id == Gtk::ResponseType::OK
        da.queue_draw # force da to use the new color now
        widget.destroy
      end

      dialog.show_all
    end

    if !window.visible?
      window.show_all
    else
      window.destroy
    end
    window
  end
end
