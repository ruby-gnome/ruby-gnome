# Copyright (c) 2015-2016 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
= Pango/Text Mask

This demo shows how to use PangoCairo to draw text with more than
just a single color.
=end
class TextmaskDemo
  def initialize(_main_window)
    @window = Gtk::Window.new(:toplevel)
    @window.resizable = true
    @window.set_size_request(400, 200)
    @window.title = "Text Mask"

    da = Gtk::DrawingArea.new

    @window.add(da)

    da.signal_connect "draw" do |_widget, cr|
      cr.save

      layout = da.create_pango_layout("Pango power!\nPango power!\nPango power!")
      desc = Pango::FontDescription.new("sans bold 34")
      layout.font_description = desc

      cr.move_to(30, 20)
      cr.pango_layout_path(layout)

      pattern = Cairo::LinearPattern.new(0.0, 0.0,
                                         da.allocated_width,
                                         da.allocated_height)

      pattern.add_color_stop(0.0, 1.0, 0.0, 0.0)
      pattern.add_color_stop(0.2, 1.0, 0.0, 0.0)
      pattern.add_color_stop(0.3, 1.0, 1.0, 0.0)
      pattern.add_color_stop(0.4, 0.0, 1.0, 0.0)
      pattern.add_color_stop(0.6, 0.0, 1.0, 1.0)
      pattern.add_color_stop(0.7, 0.0, 0.0, 1.0)
      pattern.add_color_stop(0.8, 1.0, 0.0, 1.0)
      pattern.add_color_stop(1.0, 1.0, 0.0, 1.0)
      cr.set_source(pattern)
      cr.fill_preserve

      cr.set_source_rgb(0.0, 0.0, 0.0)
      cr.set_line_width(0.5)
      cr.stroke
      cr.restore
      true
    end
  end

  def run
    if !@window.visible?
      @window.show_all
    else
      @window.destroy
    end
    @window
  end
end
