# Copyright (c) 2005 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
# $Id: rotated_text.rb,v 1.2 2005/03/05 15:01:16 mutoh Exp $
=begin
= Rotated Text

This demo shows how to use GDK and Pango to draw rotated and transformed text. 
The use of Gdk::PangoRenderer in this example is a somewhat advanced technique; most applications can simply use Gdk::Drawable#draw_layout. 
We use it here mostly because that allows us to work in user coordinates - that is, coordinates prior to the application of the transformation matrix, rather than device coordinates.
As of GTK+-2.6, the ability to draw transformed and anti-aliased graphics as shown in this example is only present for text. 
With GTK+-2.8, a new graphics system called "Cairo" will be introduced that provides these capabilities and many more for all types of graphics.
=end
require 'common'

module Demo
  class RotatedText < BasicWindow
    RADIUS = 150
    N_WORDS = 10
    FONT = "Sans Bold 27"

    def initialize
      super('Rotated Text')

      unless Gtk.check_version?(2, 6, 0)
         add(Gtk::Label.new("This sample requires GTK+ 2.6.0 or later"))
         return
      end

      drawing_area = Gtk::DrawingArea.new
      add(drawing_area)
      drawing_area.modify_bg(Gtk::STATE_NORMAL, Gdk::Color.new(65535, 65535, 65535))

      drawing_area.signal_connect("expose_event") do
	matrix = Pango::Matrix.new

	# Get the default renderer for the screen, and set it up for drawing
	renderer = Gdk::PangoRenderer.get_default(drawing_area.screen)
	renderer.drawable = drawing_area.window
	renderer.gc = style.black_gc
	width = drawing_area.allocation.width
	height = drawing_area.allocation.height

	# Set up a transformation matrix so that the user space coordinates for
	# the centered square where we draw are [-RADIUS, RADIUS], [-RADIUS, RADIUS]
	# We first center, then change the scale
	device_radius = [width, height].min / 2.0

	matrix.translate!(device_radius + (width - 2 * device_radius) / 2.0,
			  device_radius + (height - 2 * device_radius) / 2.0)
	matrix.scale!(device_radius / RADIUS, device_radius / RADIUS)

	# Create a PangoLayout, set the font and text
	context = Gdk::Pango.context
	layout = Pango::Layout.new(context)
	layout.text = "Text"
	layout.font_description = Pango::FontDescription.new(FONT)

	# Draw the layout N_WORDS times in a circle
	(0...N_WORDS).each do |i|
	  rotated_matrix = matrix.dup
	  angle = 360 * i / N_WORDS.to_f

	  # Gradient from red at angle == 60 to blue at angle == 300
	  red = 65535 * (1 + Math.cos((angle - 60) * Math::PI / 180.0)) / 2
	  color = Gdk::Color.new(red, 0, 65535 - red) 
	  renderer.set_override_color(Pango::Renderer::PART_FOREGROUND, color)

	  rotated_matrix.rotate!(angle)
	  context.matrix = rotated_matrix

	  # Inform Pango to re-layout the text with the new transformation matrix
	  layout.context_changed
	  width, height = layout.size
	  renderer.draw_layout(layout, - width / 2, - RADIUS * Pango::SCALE)
	end
	# Clean up default renderer, since it is shared
	renderer.set_override_color(Gdk::PangoRenderer::PART_FOREGROUND, nil)
	renderer.drawable = nil
	renderer.gc = nil
      end
    end
  end
end
