#!/usr/bin/env ruby
#
# This sample code is a port of
# goocanvas/demo/table-demo.c. It is licensed
# under the terms of the GNU Library General Public License, version
# 2 or (at your option) later.
#
# Copyright (C) 2013  Ruby-GNOME2 Project Team
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

require "goocanvas"

DEMO_RECT_ITEM = 0
DEMO_TEXT_ITEM = 1
DEMO_WIDGET_ITEM = 2

def create_demo_item(table, demo_item_type, row, column, rows, columns, text)
  case demo_item_type
  when DEMO_RECT_ITEM
    item = Goo::CanvasRect.new(:parent     => table,
                               :x          => 0,
                               :y          => 0,
                               :width      => 38,
                               :height     => 19,
                               :fill_color => "red")
  when DEMO_TEXT_ITEM
    item = Goo::CanvasText.new(:parent => table,
                               :text   => text,
                               :x      => 0,
                               :y      => 0,
                               :width  => -1,
                               :anchor => :nw)
  when DEMO_WIDGET_ITEM
    widget = Gtk::Button.new(:label => text)
    item = Goo::CanvasWidget.new(:parent => table,
                                 :widget => widget,
                                 :x      => 0,
                                 :y      => 0,
                                 :width  => -1,
                                 :height => -1)
  end

  table.set_child_property(item, "row", row)
  table.set_child_property(item, "column", column)
  table.set_child_property(item, "rows", rows)
  table.set_child_property(item, "columns", columns)
  table.set_child_property(item, "x-expand", true)
  table.set_child_property(item, "x-fill", true)
  table.set_child_property(item, "y-expand", true)
  table.set_child_property(item, "y-fill", true)

  item.signal_connect('button_press_event') do |item, target, event|
    puts "#{text} received 'button-press' signal at #{event.x}, #{event.y} (root:  #{event.x_root}, #{event.y_root})"
    true
  end
end

def create_table(parent, row, column, embedding_level, x, y, rotation, scale, demo_item_type)
  # Add a few simple items.
  table = Goo::CanvasTable.new(:parent         => parent,
                               :row_spacing    => 4.0,
                               :column_spacing => 4.0)

  table.translate(x, y)
  table.rotate(rotation, 0, 0)
  table.scale(scale, scale)

  if row
    parent.set_child_property(table, "row", row)
    parent.set_child_property(table, "column", column)
    parent.set_child_property(table, "x-expand", true)
    parent.set_child_property(table, "x_fill", true)
  end

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
  table = Goo::CanvasTable.new(:parent         => root,
                               :row_spacing    => 4.0,
                               :column_spacing => 4.0,
                               :width          => width,
                               :height         => height)
  table.translate(x, y)

  square = Goo::CanvasRect.new(:parent     => table,
                               :x          => 0.0,
                               :y          => 0.0,
                               :width      => 50.0,
                               :height     => 50.0,
                               :fill_color => "red")
  table.set_child_property(square, "row", 0)
  table.set_child_property(square, "column", 0)
  table.set_child_property(square, "x_shrink", true)
  square.signal_connect('button_press_event') do |item, target, event|
    puts "Red square received 'button-press' signal at #{event.x}, #{event.y} (root:  #{event.x_root}, #{event.y_root})"
    true
  end

  circle = Goo::CanvasEllipse.new(:parent     => table,
                                  :x          => 0.0,
                                  :y          => 0.0,
                                  :radius_x   => 25.0,
                                  :radius_y   => 25.0,
                                  :fill_color => "blue")
  table.set_child_property(circle, "row", 0)
  table.set_child_property(circle, "column", 1)
  table.set_child_property(circle, "x_shrink", true)
  circle.signal_connect('button_press_event') do |item, target, event|
    puts "Blue circle received 'button-press' signal at #{event.x}, #{event.y} (root:  #{event.x_root}, #{event.y_root})"
    true
  end

  points = Goo::CanvasPoints.new(3)
  points.set_point(0, 25.0,  0.0)
  points.set_point(1,  0.0, 50.0)
  points.set_point(2, 50.0, 50.0)
  triangle = Goo::CanvasPolyline.new(:parent     => table,
                                     :close_path => true,
                                     :points     => points,
                                     :fill_color => "yellow")
  table.set_child_property(triangle, "row", 0)
  table.set_child_property(triangle, "column", 2)
  table.set_child_property(triangle, "x_shrink", true)
  triangle.signal_connect('button_press_event') do |item, target, event|
    puts "Yellow triangle received 'button-press' signal at #{event.x}, #{event.y} (root:  #{event.x_root}, #{event.y_root})"
    true
  end
end

window = Gtk::Window.new(:toplevel)
window.set_default_size(640, 600)
window.signal_connect('delete_event') { Gtk.main_quit }

vbox = Gtk::Box.new(:vertical, 4)
vbox.border_width = 4
vbox.show
window.add(vbox)

hbox = Gtk::Box.new(:horizontal, 4)
vbox.pack_start(hbox, :expand  => false,
                      :fill    => false,
                      :padding => 0)
hbox.show

scrolled_win = Gtk::ScrolledWindow.new
scrolled_win.shadow_type = :in
scrolled_win.show
vbox.pack_start(scrolled_win, :expand  => true,
                              :fill    => true,
                              :padding => 0)

canvas = Goo::Canvas.new
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

