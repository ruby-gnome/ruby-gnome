# Copyright (c) 2016 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
=  Overlay/Transparency

 Use transparent background on GdkWindows to create a shadow effect on a GtkOverlay widget.
=end
class TransparentDemo
  SHADOW_OFFSET_X = 7
  SHADOW_OFFSET_Y = 7
  SHADOW_RADIUS = 5

  def initialize(main_window)
    @window = Gtk::Window.new(:toplevel)
    @window.screen = main_window.screen
    @window.set_default_size(450, 450)
    @window.title = "Transparency"

    view = Gtk::TextView.new
    sw = Gtk::ScrolledWindow.new
    sw.set_policy(:automatic, :automatic)
    sw.add(view)

    overlay = Gtk::Overlay.new
    overlay.add(sw)
    @window.add(overlay)

    entry = Gtk::Entry.new
    css = <<-CSS
    * { border-width: 0px #{SHADOW_OFFSET_X}px #{SHADOW_OFFSET_Y}px 0px; }
    CSS
    provider = Gtk::CssProvider.new
    provider.load_from_data(css)
    style_context = entry.style_context
    style_context.add_provider(provider,
                               Gtk::StyleProvider::PRIORITY_APPLICATION)
    entry.signal_connect "draw" do |widget, cr|
      allocation = widget.allocation
      rect = [allocation.x + SHADOW_OFFSET_X,
              allocation.y + SHADOW_OFFSET_Y,
              allocation.width - SHADOW_OFFSET_X,
              allocation.height - SHADOW_OFFSET_Y
             ]
      draw_shadow_box(cr, rect, SHADOW_RADIUS, 0.4)
      false
    end

    overlay.add_overlay(entry)
    entry.valign = :center
    entry.halign = :start
    overlay.show_all
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

  def draw_shadow_box(cr, rect, radius, transparency)
    x0 = rect[0]
    x1 = rect[0] + radius
    x2 = rect[0] + rect[2] - radius
    x3 = rect[0] + rect[2]

    y0 = rect[1]
    y1 = rect[1] + radius
    y2 = rect[1] + rect[3] - radius
    y3 = rect[1] + rect[3]

    # Fill non-border part
    cr.set_source_rgba(0, 0, 0, transparency)
    cr.rectangle(x1, y1, x2 - x1, y2 - y1)

    # Upper border
    pattern = Cairo::LinearPattern.new(0, y0, 0, y1)
    pattern.add_color_stop_rgba(0.0, 0.0, 0.0, 0.0)
    pattern.add_color_stop_rgba(1.0, 0.0, 0.0, transparency)

    cr.set_source(pattern)
    cr.rectangle(x1, y0, x2 - x1, y1 - y0)
    cr.fill

    # Bottom border
    pattern = Cairo::LinearPattern.new(0, y2, 0, y3)
    pattern.add_color_stop_rgba(0.0, 0.0, 0.0, transparency)
    pattern.add_color_stop_rgba(1.0, 0.0, 0.0, 0.0)
    cr.set_source(pattern)
    cr.rectangle(x1, y2, x2 - x1, y3 - y2)
    cr.fill

    # Left border
    pattern = Cairo::LinearPattern.new(x0, 0, x1, 0)
    pattern.add_color_stop_rgba(0.0, 0.0, 0.0, 0.0)
    pattern.add_color_stop_rgba(1.0, 0.0, 0.0, transparency)
    cr.set_source(pattern)
    cr.rectangle(x0, y1, x1 - x0, y2 - y1)
    cr.fill

    # Right border
    pattern = Cairo::LinearPattern.new(x2, 0, x3, 0)
    pattern.add_color_stop_rgba(0.0, 0.0, 0.0, transparency)
    pattern.add_color_stop_rgba(1.0, 0.0, 0.0, 0.0)
    cr.set_source(pattern)
    cr.rectangle(x2, y1, x3 - x2, y2 - y1)
    cr.fill

    # NW corner
    pattern = Cairo::RadialPattern.new(x1, y1, 0, x1, y1, radius)
    pattern.add_color_stop_rgba(0.0, 0.0, 0.0, transparency)
    pattern.add_color_stop_rgba(1.0, 0.0, 0.0, 0.0)
    cr.set_source(pattern)
    cr.rectangle(x0, y0, x1 - x0, y1 - y0)
    cr.fill

    # NE corner
    pattern = Cairo::RadialPattern.new(x2, y1, 0, x2, y1, radius)
    pattern.add_color_stop_rgba(0.0, 0.0, 0.0, transparency)
    pattern.add_color_stop_rgba(1.0, 0.0, 0.0, 0.0)
    cr.set_source(pattern)
    cr.rectangle(x2, y0, x3 - x2, y1 - y0)
    cr.fill

    # SW corner
    pattern = Cairo::RadialPattern.new(x1, y2, 0, x1, y2, radius)
    pattern.add_color_stop_rgba(0.0, 0.0, 0.0, transparency)
    pattern.add_color_stop_rgba(1.0, 0.0, 0.0, 0.0)
    cr.set_source(pattern)
    cr.rectangle(x0, y2, x1 - x0, y3 - y2)
    cr.fill

    # SE corner
    pattern = Cairo::RadialPattern.new(x2, y2, 0, x2, y2, radius)
    pattern.add_color_stop_rgba(0.0, 0.0, 0.0, transparency)
    pattern.add_color_stop_rgba(1.0, 0.0, 0.0, 0.0)
    cr.set_source(pattern)
    cr.rectangle(x2, y2, x3 - x2, y3 - y2)
    cr.fill
  end
end
