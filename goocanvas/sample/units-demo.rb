#!/usr/bin/env ruby
require 'goocanvas'

DATA = {
  Gtk::PaperSize::UNIT_PIXEL  => [ 100, 100, 200,  20,   10, 200, 310,  24 ],
  Gtk::PaperSize::UNIT_POINTS => [ 100, 100, 200,  20,   10, 200, 310,  24 ],
  Gtk::PaperSize::UNIT_INCH   => [   1,   1,   3, 0.5, 0.16,   3,   4, 0.3 ],
  Gtk::PaperSize::UNIT_MM     => [  30,  30, 100,  10,    5,  80,  60,  10 ],
}

def setup_canvas(canvas, units, units_name)
  d = DATA[units]

  root = canvas.root_item
  
  item = Goo::CanvasRect.new(root, d[0], d[1], d[2], d[3])
  item.signal_connect('motion_notify_event') { puts "received 'motion-notify' signal" }
  
  item = Goo::CanvasText.new(root, "This box is #{d[2]}x#{d[3]} #{units_name}",
    d[0] + d[2] / 2.0, d[1] + d[3] / 2.0, -1, Gtk::ANCHOR_CENTER, :font => "Sans #{d[4]}px")
  
  item = Goo::CanvasText.new(root, "This font is #{d[7]} #{units_name} high",
    d[5], d[6], -1, Gtk::ANCHOR_CENTER, :font => "Sans #{d[7]}px")
end

def create_canvas(units, units_name)
  vbox = Gtk::VBox.new(false, 4)
  vbox.show

  hbox = Gtk::HBox.new(false, 4)
  vbox.pack_start(hbox, false, false, 0)
  hbox.show

  canvas = Goo::Canvas.new

  w = Gtk::Label.new('Zoom:')
  hbox.pack_start(w, false, false, 0)
  w.show
  
  adj = Gtk::Adjustment.new(1, 0.05, 100, 0.05, 0.5, 0.5)
  w = Gtk::SpinButton.new(adj, 0, 2)
  adj.signal_connect('value_changed') { |new_adj| canvas.scale = new_adj.value }

  w.set_size_request(50, -1)
  hbox.pack_start(w, false, false, 0)
  w.show

  scrolled_win = Gtk::ScrolledWindow.new
  vbox.pack_start(scrolled_win, true, true, 0)
  scrolled_win.show

  # Create the canvas.
  canvas.set_size_request(600, 450)
  setup_canvas(canvas, units, units_name)

  canvas.set_bounds(0, 0, 1000, 1000)
  canvas.units = units
  canvas.anchor = Gtk::ANCHOR_CENTER
  canvas.show
  
  scrolled_win.add(canvas)

  vbox
end

window = Gtk::Window.new(Gtk::Window::TOPLEVEL)
window.set_default_size(640, 600)
window.show
window.signal_connect('delete_event') { Gtk.main_quit }

notebook = Gtk::Notebook.new
notebook.show
window.add(notebook)

notebook.append_page(create_canvas(Gtk::PaperSize::UNIT_PIXEL,  'pixels'),      Gtk::Label.new('Pixels'))
notebook.append_page(create_canvas(Gtk::PaperSize::UNIT_POINTS, 'points'),      Gtk::Label.new('Points'))
notebook.append_page(create_canvas(Gtk::PaperSize::UNIT_INCH,   'inches'),      Gtk::Label.new('Inches'))
notebook.append_page(create_canvas(Gtk::PaperSize::UNIT_MM,     'millimeters'), Gtk::Label.new('Millimeters'))

Gtk.main