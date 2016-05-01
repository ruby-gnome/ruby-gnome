# Copyright (c) 2016 Ruby-GNOME2 Project Team
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
module RotatedTextDemo
  HEART = "♥"
  RADIUS = 150
  N_WORDS = 5
  FONT = "Serif 18"
  TEXT = "I ♥ GTK+"
  
  def self.run_demo(main_window)
    window = Gtk::Window.new(:toplevel)
    window.screen = main_window.screen
    window.title = "Rotated Text"
    window.set_default_size(4 * RADIUS, 2 * RADIUS)
    
    box = Gtk::Box.new(:horizontal, 0)
    box.homogeneous = true
    window.add(box)

    # Add adrawing area
    drawing_area = Gtk::DrawingArea.new
    box.add(drawing_area)
    drawing_area.style_context.add_class("view")

    drawing_area.signal_connect "draw" do |widget|
      # rotated_text_draw
    end

    # And a label
    label = Gtk::Label.new(TEXT)
    box.add(label)
    label.angle = 45

    # Set up fancy stuff on the label
    layout = label.layout
    puts layout.class
    puts layout.methods
    if !window.visible?
      window.show_all
    else
      window.destroy
    end
    window
  end
end
