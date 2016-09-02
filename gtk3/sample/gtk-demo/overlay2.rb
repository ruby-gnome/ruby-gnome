# Copyright (c) 2015-2016 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
= Overlay/Decorative Overlay

Another example of an overlay with some decorative
and some interactive controls.
=end
class Overlay2Demo
  def initialize(_main_window)
    @window = Gtk::Window.new(:toplevel)
    @window.set_default_size(500, 510)
    @window.title = "Decorative Overlay"

    @overlay = Gtk::Overlay.new

    sw = Gtk::ScrolledWindow.new
    sw.set_policy(:automatic, :automatic)

    initialize_textview

    @window.add(@overlay)
    @overlay.add(sw)
    sw.add(@text)

    generate_image_in_overlay("/overlay2/decor1.png", :start)
    generate_image_in_overlay("/overlay2/decor2.png", :end)

    generate_adjustment
    generate_scale_in_overlay

    @overlay.show_all
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

  def generate_image_in_overlay(resource, position)
    image = Gtk::Image.new(:resource => resource)
    @overlay.add_overlay(image)
    @overlay.set_overlay_pass_through(image, true)
    image.halign = position
    image.valign = position
  end

  def generate_adjustment
    @adjustment = Gtk::Adjustment.new(0, 0, 100, 1, 1, 0)
    @adjustment.signal_connect "value-changed" do |widget|
      value = widget.value
      @text.set_left_margin(value)
      @tag.set_property("pixels-above-lines", value)
    end
    @adjustment.value = 100
  end

  def generate_scale_in_overlay
    scale = Gtk::Scale.new(:horizontal, @adjustment)
    scale.draw_value = false
    scale.set_size_request(120, -1)
    scale.margin_start = 20
    scale.margin_end = 20
    scale.margin_bottom = 20
    @overlay.add_overlay(scale)
    scale.halign = :start
    scale.valign = :end
    scale.tooltip_text = "Margin"
  end

  def initialize_textview
    @text = Gtk::TextView.new
    buffer = @text.buffer
    buffer.text = "Dear Diary..."

    @tag = buffer.create_tag("top-margin", "pixels-above-lines" => 0)

    word_end = buffer.start_iter
    word_end.forward_word_end
    buffer.apply_tag(@tag, buffer.start_iter, word_end)
  end
end
