# Copyright (c) 2015-2016 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
= Color Chooser

A GtkColorChooser lets the user choose a color. There are several
implementations of the GtkColorChooser interface in GTK+. The
GtkColorChooserDialog is a prebuilt dialog containing a
GtkColorChooserWidget.
=end
class ColorselDemo
  def initialize(main_window)
    @color = Gdk::RGBA.new(0, 0, 1, 1)

    @window = Gtk::Window.new(:toplevel)
    @window.screen = main_window.screen
    @window.title = "Color Chooser"

    vbox = Gtk::Box.new(:vertical, 8)
    vbox.margin = 12
    @window.add(vbox)
    frame = initialize_drawing_area_frame
    vbox.pack_start(frame, :expand => true, :fill => true, :padding => 0)

    button = initialize_color_chooser_button
    vbox.pack_start(button, :expand => false, :fill => false, :padding => 0)
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

  def initialize_drawing_area_frame
    frame = Gtk::Frame.new
    frame.shadow_type = :in
    @da = Gtk::DrawingArea.new
    @da.signal_connect "draw" do |_widget, cr|
      cr.set_source(@color.to_a)
      cr.paint
    end
    @da.set_size_request(200, 200)
    frame.add(@da)
    frame
  end

  def initialize_color_chooser_button
    button = Gtk::Button.new(:mnemonic => "_Change the above color")
    button.set_halign(:end)
    button.set_valign(:center)

    button.signal_connect "clicked" do |_widget|
      generate_color_chooser_dialog
    end
    button
  end

  def generate_color_chooser_dialog
    dialog = Gtk::ColorChooserDialog.new(:title => "Changing Color",
                                           :parent => @window)
    dialog.modal = true
    dialog.rgba = @color

    dialog.signal_connect "response" do |widget, response_id|
      @color = widget.rgba if response_id == Gtk::ResponseType::OK
      @da.queue_draw # force da to use the new color now
      widget.destroy
    end

    dialog.show_all
  end
end
