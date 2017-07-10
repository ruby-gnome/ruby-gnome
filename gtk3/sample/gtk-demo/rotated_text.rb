# Copyright (c) 2016-2017 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
=  Pango/Rotated Text

 This demo shows how to use PangoCairo to draw rotated and transformed
 text. The right pane shows a rotated GtkLabel widget.

 In both cases, a custom PangoCairo shape renderer is installed to draw
 a red heard using cairo drawing operations instead of the Unicode heart
 character.
=end
class RotatedTextDemo
  HEART = "♥"
  RADIUS = 150
  N_WORDS = 5
  FONT = "Serif 18"
  TEXT = "I ♥ GTK+"

  def initialize(main_window)
    @window = Gtk::Window.new(:toplevel)
    @window.screen = main_window.screen
    @window.title = "Rotated Text"
    @window.set_default_size(4 * RADIUS, 2 * RADIUS)

    box = Gtk::Box.new(:horizontal, 0)
    box.homogeneous = true
    @window.add(box)

    # Add adrawing area
    drawing_area = Gtk::DrawingArea.new
    box.add(drawing_area)
    drawing_area.style_context.add_class("view")

    drawing_area.signal_connect "draw" do |widget, cr|
      translate_and_scale(widget, cr)

      add_gradient(cr)

      layout = initialize_da_pango_layout(widget)

      draw_the_rotated_texts(cr, layout)

      false
    end

    # And a label
    label = Gtk::Label.new(TEXT)
    box.add(label)
    label.angle = 45

    # Set up fancy stuff on the label
    layout = label.layout
    layout.context.set_shape_renderer do |cr, attr, do_path|
      fancy_shape_renderer(cr, attr, do_path)
    end

    label.attributes = create_fancy_attr_list_for_layout(layout)
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

  def fancy_shape_renderer(cr, attr, do_path)
    x, y = cr.current_point
    cr.translate(x, y)
    rect = attr.ink_rect
    cr.scale(rect.width / Pango::SCALE,
             rect.height / Pango::SCALE)

    if attr.data == HEART
      cr.move_to(0.5, 0)
      cr.line_to(0.9, -0.4)
      cr.curve_to(1.1, -0.8, 0.5, -0.9, 0.5, -0.5)
      cr.curve_to(0.5, -0.9, -0.1, -0.8, 0.1, -0.4)
      cr.close_path
    end

    unless do_path
      cr.set_source_rgb(1, 0, 0)
      cr.fill
    end
  end

  def create_fancy_attr_list_for_layout(layout)
    metrics = layout.context.get_metrics(layout.font_description)
    ascent = metrics.ascent
    logical_rect = Pango::Rectangle.new(0, -ascent, ascent, ascent)
    ink_rect = Pango::Rectangle.new(0, -ascent, ascent, ascent)

    attrs = Pango::AttrList.new
    attr = Pango::AttrShape.new(ink_rect, logical_rect, HEART)
    attr.start_index = TEXT[0, TEXT.index(HEART)].bytesize
    attr.end_index = attr.start_index + HEART.bytesize
    attrs.insert(attr)
    attrs
  end

  def translate_and_scale(widget, cr)
    # Create a cairo context and set up a transformation matrix so that the
    # user space coordinates for the centered square where we draw are
    # [-RADIUS, RADIUS], [-RADIUS, RADIUS].
    # We first center, then change the scale.
    width = widget.allocated_width
    height = widget.allocated_width
    device_radius = [width, height].min / 2
    cr.translate(device_radius + (width - 2 * device_radius) / 2,
                 device_radius + (height - 2 * device_radius) / 2)
    cr.scale(device_radius / RADIUS, device_radius / RADIUS)
  end

  def add_gradient(cr)
    # Create a subtle gradient source and use it
    pattern = Cairo::LinearPattern.new(-RADIUS, -RADIUS, RADIUS, RADIUS)
    pattern.add_color_stop_rgb(0, 0.5, 0, 0)
    pattern.add_color_stop_rgb(1, 0, 0, 0.5)
    cr.set_source(pattern)
  end

  def initialize_da_pango_layout(widget)
    # Create a PangoContext and set up our shape renderer
    context = widget.create_pango_context
    context.set_shape_renderer do |cairo, attr, do_path|
      fancy_shape_renderer(cairo, attr, do_path)
    end

    # Create a PangoLayout, set the text, font and attributes
    layout = Pango::Layout.new(context)
    layout.text = TEXT
    desc = Pango::FontDescription.new(FONT)
    layout.font_description = desc

    attrs = create_fancy_attr_list_for_layout(layout)
    layout.attributes = attrs
    layout
  end

  def draw_the_rotated_texts(cr, layout)
    # Draw the layout N_WORDS times in a circle
    N_WORDS.times do
      # inform Pango to re-layout the text with the new transformation matrix
      cr.update_pango_layout(layout)
      w, = layout.pixel_size
      cr.move_to(- w / 2, - RADIUS * 0.9)
      cr.show_pango_layout(layout)
      cr.rotate(Math::PI * 2 / N_WORDS)
    end
  end
end
