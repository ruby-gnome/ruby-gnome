#!/usr/bin/env ruby
#
# This sample code is a port of
# goocanvas/demo/simple-demo.c. It is licensed
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

require 'goocanvas-gi'

Goo.init

# Create the window and widgets.
window = Gtk::Window.new(:toplevel)
window.set_default_size(640, 600)
window.show
window.signal_connect('delete_event') { Gtk.main_quit }

scrolled_win = Gtk::ScrolledWindow.new
scrolled_win.shadow_type = :in
scrolled_win.show
window.add(scrolled_win)

canvas = Goo::Canvas.new
canvas.set_size_request(600, 450)
canvas.set_bounds(0, 0, 1000, 1000)
canvas.show
scrolled_win.add(canvas)

root = canvas.root_item

rect_item = Goo::CanvasRect.new(:parent       => root,
                                :x            => 100,
                                :y            => 100,
                                :width        => 400,
                                :height       => 400,
                                :line_width   => 10.0,
                                :radius_x     => 20.0,
                                :radius_y     => 10.0,
                                :stroke_color => 'yellow',
                                :fill_color   => 'red')

rect_item.signal_connect('button_press_event') do
  puts "rect item received button press event"
end

text_item = Goo::CanvasText.new(:parent => root,
                                :text   => 'Hello World',
                                :x      => 300,
                                :y      => 300,
                                :width  => -1,
                                :anchor => :center,
                                :font   => 'Sans 24')

text_item.rotate(45, 300, 300)

Gtk.main
