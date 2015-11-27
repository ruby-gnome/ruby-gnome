# Copyright (c) 2015 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
= Overlay/Decorative Overlay

Another example of an overlay with some decorative
and some interactive controls.
=end
module Overlay2Demo
  def self.run_demo(_main_window)
    window = Gtk::Window.new(:toplevel)
    window.set_default_size(500, 510)
    window.set_title("Decorative Overlay")

    overlay = Gtk::Overlay.new

    sw = Gtk::ScrolledWindow.new
    sw.set_policy(:automatic, :automatic)

    text = Gtk::TextView.new
    buffer = text.buffer
    buffer.text = "Dear Diary..."

    tag = buffer.create_tag("top-margin", "pixels-above-lines" => 0)

    word_end = buffer.start_iter
    word_end.forward_word_end
    buffer.apply_tag(tag, buffer.start_iter, word_end)

    window.add(overlay)
    overlay.add(sw)
    sw.add(text)

    image = Gtk::Image.new(:resource => "/overlay2/decor1.png")
    overlay.add_overlay(image)
    overlay.set_overlay_pass_through(image, true)
    image.set_halign(:start)
    image.set_valign(:start)

    image = Gtk::Image.new(:resource => "/overlay2/decor2.png")
    overlay.add_overlay(image)
    overlay.set_overlay_pass_through(image, true)
    image.set_halign(:end)
    image.set_valign(:end)

    adjustment = Gtk::Adjustment.new(0, 0, 100, 1, 1, 0)
    adjustment.signal_connect "value-changed" do |widget|
      value = widget.value
      text.set_left_margin(value)
      tag.set_property("pixels-above-lines", value)
    end

    scale = Gtk::Scale.new(:horizontal, adjustment)
    scale.set_draw_value(false)
    scale.set_size_request(120, -1)
    scale.set_margin_start(20)
    scale.set_margin_end(20)
    scale.set_margin_bottom(20)
    overlay.add_overlay(scale)
    scale.set_halign(:start)
    scale.set_valign(:end)
    scale.set_tooltip_text("Margin")

    adjustment.value = 100
    overlay.show_all

    if !window.visible?
      window.show_all
    else
      window.destroy
    end
    window
  end
end
