=begin header

  gnome-canvas.rb - Affine transformation sample on Gnome::Canvas.

  $Author: tkubo $
  $Date: 2002/09/20 14:51:20 $

  Copyright (C) 2002  KUBO Takehiro <kubo@jiubao.org>

=end

require 'libart'
require 'gnome'

class AffineApp < Gnome::App
  include Gnome::I18n

  CANVAS_SIZE_X = 200
  CANVAS_SIZE_Y = 100
  TRANSLATE_X = 50
  TRANSLATE_Y = 20

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

    canvas = Gnome::Canvas.new_aa
    canvas.set_usize(CANVAS_SIZE_X, CANVAS_SIZE_Y)
    canvas.set_scroll_region(0, 0, CANVAS_SIZE_X, CANVAS_SIZE_Y)
    frame.add(canvas)
    canvas.show()
    points = Gnome::CanvasPoints.new(2)
    points[0] = CANVAS_SIZE_X / 2
    points[1] = 0
    points[2] = CANVAS_SIZE_X / 2
    points[3] = CANVAS_SIZE_Y
    canvas.root.item_new(Gnome::CanvasLine,
			 "points", points,
			 "fill_color", "gray",
			 "width_pixels", 1)
    points[0] = 0
    points[1] = CANVAS_SIZE_Y / 2
    points[2] = CANVAS_SIZE_X
    points[3] = CANVAS_SIZE_Y / 2
    canvas.root.item_new(Gnome::CanvasLine,
			 "points", points,
			 "fill_color", "gray",
			 "width_pixels", 1)
    text = canvas.root.item_new(Gnome::CanvasText,
				"text", "Hello, Ruby/GNOME.",
				"x", 0,
				"y", 0,
				"font", "-b&h-lucida-bold-r-normal-*-14-*-*-*-p-*-iso8859-1",
				"anchor", Gtk::ANCHOR_CENTER,
				"fill_color", "firebrick")
    text.affine_relative(TRANS * affine)
  end

  def initialize
    super("affine-on-gnome-canvas", "Affine transformation test on Gnome::Canvas")

    file_menu = [
      [ Gnome::App::UI_ITEM, N_("Exit"), nil, proc{ Gtk::main_quit }, nil,
	Gnome::App::PIXMAP_STOCK, Gnome::Stock::MENU_EXIT, ?Q,
	Gdk::CONTROL_MASK, nil ],
    ]

    main_menu = [
      Gnome::UIInfo::subtree(N_("File"), file_menu),
    ]

    create_menus(main_menu)

    sw = Gtk::ScrolledWindow.new
    sw.set_usize(600, 600)
    set_contents(sw)
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
    show
  end
end

def main
  Gnome::init("test-gnome-canvas", "1.0")
  AffineApp.new
  Gtk::main
end

main
