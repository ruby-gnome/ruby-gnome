=begin header

  gnome-canvas.rb - Affine transformation sample using Ruby/GnomeCanvas2.

  $Author: mutoh $
  $Date: 2006/06/29 18:24:23 $

  Copyright (C) 2003-2006  Ruby-GNOME2 Project Team
  Copyright (C) 2002,2003  KUBO Takehiro <kubo@jiubao.org>

=end

require 'gnomecanvas2'

class AffineApp < Gtk::Window
  CANVAS_SIZE_X = 200
  CANVAS_SIZE_Y = 100
  TRANSLATE_X = 20
  TRANSLATE_Y = 20
  ITEM_SIZE_X = 20
  ITEM_SIZE_Y = 20

  # affine transformation to move to the center of the Canvas.
  TRANS = Art::Affine.translate(CANVAS_SIZE_X / 2, CANVAS_SIZE_Y / 2)

  def setup_canvas_frame(table, x, y, title, affine)
    frame = Gtk::Frame.new(title)
    frame.set_shadow_type(Gtk::SHADOW_IN)
    table.attach(frame,
		 x, x + 1,
		 y, y + 1,
		 Gtk::EXPAND | Gtk::FILL | Gtk::SHRINK,
		 Gtk::EXPAND | Gtk::FILL | Gtk::SHRINK,
		 0, 0)
    frame.show

    canvas = Gnome::Canvas.new()
    canvas.set_size_request(CANVAS_SIZE_X, CANVAS_SIZE_Y)
    canvas.set_scroll_region(0, 0, CANVAS_SIZE_X, CANVAS_SIZE_Y)
    frame.add(canvas)
    canvas.show()

    points = [[CANVAS_SIZE_X / 2, 0], [CANVAS_SIZE_X / 2, CANVAS_SIZE_Y]]
    Gnome::CanvasLine.new(canvas.root,
			  {:points => points,
			    :fill_color => "gray",
			    :width_pixels => 1})
    points = [[0, CANVAS_SIZE_Y / 2], [CANVAS_SIZE_X, CANVAS_SIZE_Y / 2]]
    Gnome::CanvasLine.new(canvas.root,
			  {:points => points,
			    :fill_color => "gray",
			    :width_pixels => 1})

    group = Gnome::CanvasGroup.new(canvas.root, {})
    Gnome::CanvasRect.new(group,
			  {:x1 => 0, :y1 => 0,
			    :x2 => - ITEM_SIZE_X / 2, :y2 => - ITEM_SIZE_Y / 2,
			    :fill_color => "blue"})
    Gnome::CanvasRect.new(group,
			  {:x1 => 0, :y1 => 0,
			    :x2 => ITEM_SIZE_X / 2, :y2 => - ITEM_SIZE_Y / 2,
			    :fill_color => "red"})
    Gnome::CanvasRect.new(group,
			  {:x1 => 0, :y1 => 0,
			    :x2 => - ITEM_SIZE_X / 2, :y2 => ITEM_SIZE_Y / 2,
			    :fill_color => "green"})
    Gnome::CanvasRect.new(group,
			  {:x1 => 0, :y1 => 0,
			    :x2 => ITEM_SIZE_X / 2, :y2 => ITEM_SIZE_Y / 2,
			    :fill_color => "yellow"})
    group.affine_relative(TRANS * affine)
  end

  def initialize
    super(Gtk::Window::TOPLEVEL)

    self.signal_connect("delete_event") do
      Gtk::main_quit()
    end

    sw = Gtk::ScrolledWindow.new
    sw.set_size_request(600, 400)
    self.add(sw)
    sw.show

    affine = Array.new(5)
    text = Array.new(5)
    affine[0] = Art::Affine.identity
    text[0] = 'No Conversion'
    affine[1] = Art::Affine.scale(1, 2)
    text[1] = 'Scale(1,2)'
    affine[2] = Art::Affine.rotate(45)
    text[2] = 'Rotate(45)'
    affine[3] = Art::Affine.shear(45)
    text[3] = 'Shear(45)'
    affine[4] = Art::Affine.translate(TRANSLATE_X, TRANSLATE_Y)
    text[4] = "Translate(#{TRANSLATE_X}, #{TRANSLATE_Y})"
    
    table = Gtk::Table.new(affine.size, affine.size, false)
    sw.add_with_viewport(table)
    table.show

    0.upto(affine.size - 1) do |x|
      0.upto(affine.size - 1) do |y|
	if x == 0
	  label = text[y]
	elsif y == 0
	  label = text[x]
	else
	  label = text[x] + ' * ' + text[y]
	end
	setup_canvas_frame(table, x, y, label, affine[x] * affine[y])
      end
    end
    self.show
  end
end

def main
  AffineApp.new
  Gtk::main
end

main
