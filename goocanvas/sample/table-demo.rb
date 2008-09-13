#!/usr/bin/env ruby
require 'goocanvas'
DEMO_RECT_ITEM = 0
DEMO_TEXT_ITEM = 1
DEMO_WIDGET_ITEM = 2

def create_demo_item(table, demo_item_type, row, column, rows, columns, text)
  case demo_item_type
  when DEMO_RECT_ITEM
    item = Goo::CanvasRect.new(table, 0, 0, 38, 19, :fill_color => 'red')
  when DEMO_TEXT_ITEM
    item = Goo::CanvasText.new(table, text, 0, 0, -1, Gtk::ANCHOR_NW, nil)
  when DEMO_WIDGET_ITEM
    widget = Gtk::Button.new(text)
    item = Goo::CanvasWidget.new(table, widget, 0, 0, -1, -1)
  end

  table.set_child_properties(item, :row => row, :column => column, :rows => rows, :columns => columns,
    :x_expand => true, :x_fill => true, :y_expand => true, :y_fill => true)

  item.signal_connect('button_press_event') do |item, target, event|
    puts "#{text} received 'button-press' signal at #{event.x}, #{event.y} (root:  #{event.x_root}, #{event.y_root})"
    true
  end
end

def create_table(parent, row, column, embedding_level, x, y, rotation, scale, demo_item_type)
  # Add a few simple items.
  table = Goo::CanvasTable.new(parent, :row_spacing => 4.0, :column_spacing => 4.0)

  table.translate(x, y)
  table.rotate(rotation, 0, 0)
  table.scale(scale, scale)

  parent.set_child_properties(table, :row => row, :column => column, :x_expand => true, :x_fill => true) if row

  if embedding_level > 0
    level = embedding_level - 1
    create_table(table, 0, 0, level, 50, 50, 0, 0.7, demo_item_type)
    create_table(table, 0, 1, level, 50, 50, 45, 1.0, demo_item_type)
    create_table(table, 0, 2, level, 50, 50, 90, 1.0, demo_item_type)
    create_table(table, 1, 0, level, 50, 50, 135, 1.0, demo_item_type)
    create_table(table, 1, 1, level, 50, 50, 180, 1.5, demo_item_type)
    create_table(table, 1, 2, level, 50, 50, 225, 1.0, demo_item_type)
    create_table(table, 2, 0, level, 50, 50, 270, 1.0, demo_item_type)
    create_table(table, 2, 1, level, 50, 50, 315, 1.0, demo_item_type)
    create_table(table, 2, 2, level, 50, 50, 360, 2.0, demo_item_type)
  else
    create_demo_item(table, demo_item_type, 0, 0, 1, 1, "(0,0)")
    create_demo_item(table, demo_item_type, 0, 1, 1, 1, "(1,0)")
    create_demo_item(table, demo_item_type, 0, 2, 1, 1, "(2,0)")
    create_demo_item(table, demo_item_type, 1, 0, 1, 1, "(0,1)")
    create_demo_item(table, demo_item_type, 1, 1, 1, 1, "(1,1)")
    create_demo_item(table, demo_item_type, 1, 2, 1, 1, "(2,1)")
    create_demo_item(table, demo_item_type, 2, 0, 1, 1, "(0,2)")
    create_demo_item(table, demo_item_type, 2, 1, 1, 1, "(1,2)")
    create_demo_item(table, demo_item_type, 2, 2, 1, 1, "(2,2)")
  end
  table
end

def create_demo_table(root, x, y, width, height)
  table = Goo::CanvasTable.new(root, :row_spacing => 4.0, :column_spacing => 4.0, :width => width, :height => height)
  table.translate(x, y)

  square = Goo::CanvasRect.new(table, 0.0, 0.0, 50.0, 50.0, :fill_color => 'red')
  table.set_child_properties(square, :row => 0, :column => 0, :x_shrink => true)
  square.signal_connect('button_press_event') do |item, target, event|
    puts "Red square received 'button-press' signal at #{event.x}, #{event.y} (root:  #{event.x_root}, #{event.y_root})"
    true
  end

  circle = Goo::CanvasEllipse.new(table, 0.0, 0.0, 25.0, 25.0, :fill_color => 'blue')
  table.set_child_properties(circle, :row => 0, :column => 1, :x_shrink => true)
  circle.signal_connect('button_press_event') do |item, target, event|
    puts "Blue circle received 'button-press' signal at #{event.x}, #{event.y} (root:  #{event.x_root}, #{event.y_root})"
    true
  end

  triangle = Goo::CanvasPolyline.new(table, true, [ [ 25.0, 0.0], [ 0.0, 50.0 ], [ 50.0, 50.0 ] ], :fill_color => "yellow")
  table.set_child_properties(triangle, :row => 0, :column => 2, :x_shrink => true)
  triangle.signal_connect('button_press_event') do |item, target, event|
    puts "Yellow triangle received 'button-press' signal at #{event.x}, #{event.y} (root:  #{event.x_root}, #{event.y_root})"
    true
  end
end

window = Gtk::Window.new(Gtk::Window::TOPLEVEL)
window.set_default_size(640, 600)
window.signal_connect('delete_event') { Gtk.main_quit }

vbox = Gtk::VBox.new(false, 4)
vbox.border_width = 4
vbox.show
window.add(vbox)

hbox = Gtk::HBox.new(false, 4)
vbox.pack_start(hbox, false, false, 0)
hbox.show

scrolled_win = Gtk::ScrolledWindow.new
scrolled_win.shadow_type = Gtk::SHADOW_IN
scrolled_win.show
vbox.pack_start(scrolled_win, true, true, 0)

canvas = Goo::Canvas.new
canvas.flags = Gtk::Widget::CAN_FOCUS
canvas.set_size_request(600, 450)
canvas.set_bounds(0, 0, 1000, 1000)
scrolled_win.add(canvas)

root = canvas.root_item

create_demo_table(root, 400, 200, -1, -1)
create_demo_table(root, 400, 260, 100, -1)

create_table(root, nil, nil, 0, 10, 10, 0, 1.0, DEMO_TEXT_ITEM)
create_table(root, nil, nil, 0, 180, 10, 30, 1.0, DEMO_TEXT_ITEM)
create_table(root, nil, nil, 0, 350, 10, 60, 1.0, DEMO_TEXT_ITEM)
create_table(root, nil, nil, 0, 500, 10, 90, 1.0, DEMO_TEXT_ITEM)

table = create_table(root, nil, nil, 0, 30, 150, 0, 1.0, DEMO_TEXT_ITEM)
table.width = 300.0
table.height = 100.0

create_table(root, nil, nil, 1, 200, 200, 30, 0.8, DEMO_TEXT_ITEM)

table = create_table(root, nil, nil, 0, 10, 700, 0, 1.0, DEMO_WIDGET_ITEM)
table.width = 300.0
table.height = 200.0

canvas.show

window.show

Gtk.main

