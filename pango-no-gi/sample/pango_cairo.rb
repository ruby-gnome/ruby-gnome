=begin
  cairo.rb - Ruby/Pango with cairo sample script.

  Copyright (c) 2005 Ruby-GNOME2 Project
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: pango_cairo.rb,v 1.1 2005/10/15 07:57:52 mutoh Exp $
=end

require 'pango'

unless Pango.cairo_available?
  STDERR.puts "This sample requires Pango 1.0.0 or later and cairo support"
  exit 1
end

filename = ARGV.shift
filename ||= "cairo.png"

RADIUS = 150
N_WORDS = 10
FONT = "Sans Bold 27"

def draw_text(cr)
  # Center coordinates on the middle of the region we are drawing
  cr.translate(RADIUS, RADIUS);

  # Create a PangoLayout, set the font and text
  layout = cr.create_pango_layout
  
  layout.set_text("Text")
  desc = Pango::FontDescription.new(FONT)
  layout.set_font_description(desc)

  # Draw the layout N_WORDS times in a circle
  N_WORDS.times do |i|
    angle = (360.0 * i) / N_WORDS;
    cr.save do
      # Gradient from red at angle == 60 to blue at angle == 300
      red = (1 + Math.cos((angle - 60) * Math::PI / 180.0)) / 2
      cr.set_source_rgb(red, 0, 1.0 - red)
      cr.rotate(angle * Math::PI / 180.0)
    
      # Inform Pango to re-layout the text with the new transformation
      cr.update_pango_layout(layout)
    
      width, height = layout.size
      cr.move_to(-(width.to_f / Pango::SCALE) / 2, -RADIUS)
      cr.show_pango_layout(layout)
    end
  end
end

diameter = 2 * RADIUS
surface = Cairo::ImageSurface.new(Cairo::FORMAT_ARGB32, diameter, diameter)
cr = Cairo::Context.new(surface)
cr.set_source_rgb(1.0, 1.0, 1.0)
cr.rectangle(0, 0, diameter, diameter);
cr.fill
draw_text(cr)

File.open(filename, "wb") do |f|
  cr.target.write_to_png(f)
end
# or
#  cr.target.write_to_png(filename)
