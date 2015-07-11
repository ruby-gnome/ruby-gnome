#!/usr/bin/env ruby
=begin
  pangolayout.rb - Ruby/GTK sample script.

  Copyright (c) 2005-2015  Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
=end

require "gtk3"

RADIUS =  80
N_WORDS = 16
HEIGHT = 400
WIDTH = 400

class PaintableWindow < Gtk::Window
  attr_reader :x, :y, :width, :height

  def initialize(name)
    super(name)
    set_app_paintable(true)
    set_decorated(true)
    set_default_size(WIDTH, HEIGHT)
    signal_connect("destroy") { Gtk.main_quit }
    set_double_buffered(false)

    # Get the size attributes when the window is resized
    signal_connect "size-allocate" do |_widget, rectangle|
      @x = rectangle.x
      @y = rectangle.y
      @width = rectangle.width
      @height = rectangle.height
    end

    signal_connect "screen-changed" do |widget, _old_screen|
      screen_changed(widget)
    end

    screen_changed(self)
  end

  def supports_alpha?
    @supports_alpha
  end

  private

  def screen_changed(widget)
    visual = widget.screen.rgba_visual
    if visual && widget.screen.composited?
      set_visual(visual)
      @supports_alpha = true
    else
      set_visual(widget.screen.system_visual)
      @supports_alpha = false
    end
  end
end

pangolayout = PaintableWindow.new("Pango Render")

pangolayout.signal_connect "draw" do |widget, cr|
  # Redraw on a clean destination
  # http://cairographics.org/manual/cairo-cairo-t.html#cairo-operator-t
  cr.set_operator(Cairo::OPERATOR_SOURCE)

  if widget.supports_alpha?
    cr.set_source_rgba(0, 0, 0, 0.3)
  else
    cr.set_source_rgb(1, 1, 1)
  end

  cr.paint

  device_radius = [widget.width, widget.height].min / 6.0
  cr.translate(device_radius + (widget.width - 2 * device_radius) / 2.0,
               device_radius + (widget.height - 2 * device_radius) / 2.0)
  cr.scale(device_radius / RADIUS, device_radius / RADIUS)

  pango_layout = cr.create_pango_layout
  pango_layout.text = "Ruby-GNOME2"
  pango_layout.font_description = Pango::FontDescription.new("Sans Bold 14")

  N_WORDS.times do |i|
    angle = 360 * i / N_WORDS.to_f
    cr.set_source_rgb((65_535 * rand) / 65_355,
                      (65_535 * rand) / 65_355,
                      (65_535 * rand) / 65_355)

    cr.rotate((angle * Math::PI) / 180.0)
    cr.update_pango_layout(pango_layout)
    width, _height = pango_layout.size
    cr.move_to((width / 2) / Pango::SCALE, RADIUS)
    cr.show_pango_layout(pango_layout)
  end
end

pangolayout.show

Gtk.main
