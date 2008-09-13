#!/usr/bin/env ruby
require 'goocanvas'

# Create the window and widgets.
window = Gtk::Window.new(Gtk::Window::TOPLEVEL)
window.set_default_size(640, 600)
window.show
window.signal_connect('delete_event') { Gtk.main_quit }

scrolled_win = Gtk::ScrolledWindow.new
scrolled_win.shadow_type = Gtk::SHADOW_IN
scrolled_win.show
window.add(scrolled_win)

canvas = Goo::Canvas.new
canvas.set_size_request(600, 450)
canvas.set_bounds(0, 0, 1000, 1000)
canvas.show
scrolled_win.add(canvas)

root = canvas.root_item

rect_item = Goo::CanvasRect.new(root, 100, 100, 400, 400,
  :line_width => 10.0,
  :radius_x => 20.0,
  :radius_y => 10.0,
  :stroke_color => 'yellow',
  :fill_color => 'red')
rect_item.signal_connect('button_press_event') { puts "rect item received button press event\n" }

text_item = Goo::CanvasText.new(root, 'Hello World', 300, 300, -1, Gtk::ANCHOR_CENTER, :font => 'Sans 24')

text_item.rotate(45, 300, 300)

Gtk.main
