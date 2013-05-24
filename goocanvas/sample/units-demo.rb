#!/usr/bin/env ruby
#
# This sample code is a port of
# goocanvas/demo/units-demo.c. It is licensed
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

require 'goocanvas'

DATA = {
  :pixel  => [ 100, 100, 200,  20,   10, 200, 310,  24 ],
  :points => [ 100, 100, 200,  20,   10, 200, 310,  24 ],
  :inch   => [   1,   1,   3, 0.5, 0.16,   3,   4, 0.3 ],
  :mm     => [  30,  30, 100,  10,    5,  80,  60,  10 ],
}

def setup_canvas(canvas, units, units_name)
  d = DATA[units]

  root = canvas.root_item
  
  item = Goo::CanvasRect.new(:parent => root,
                             :x      => d[0],
                             :y      => d[1],
                             :width  => d[2],
                             :height => d[3])

  item.signal_connect('motion_notify_event') do
    puts("item received 'motion-notify' signal")
  end

  item = Goo::CanvasText.new(:parent => root,
                             :text   => "This box is #{d[2]}x#{d[3]} #{units_name}",
                             :x      => d[0] + d[2] / 2.0,
                             :y      => d[1] + d[3] / 2.0,
                             :width  => -1,
                             :anchor => :center,
                             :font   => "Sans #{d[4]}px")

  item = Goo::CanvasText.new(:parent => root,
                             :text   => "This font is #{d[7]} #{units_name} high",
                             :x      => d[5],
                             :y      => d[6],
                             :width  => -1,
                             :anchor => :center,
                             :font   => "Sans #{d[7]}px")
end

def create_canvas(units, units_name)
  vbox = Gtk::Box.new(:vertical, 4)
  vbox.show

  hbox = Gtk::Box.new(:horizontal, 4)
  vbox.pack_start(hbox, :expand  => false,
                        :fill    => false,
                        :padding => 0)
  hbox.show

  canvas = Goo::Canvas.new

  w = Gtk::Label.new('Zoom:')
  hbox.pack_start(w, :expand  => false,
                     :fill    => false,
                     :padding => 0)
  w.show
  
  adj = Gtk::Adjustment.new(1, 0.05, 100, 0.05, 0.5, 0.5)
  w = Gtk::SpinButton.new(adj, 0, 2)
  adj.signal_connect('value_changed') do |new_adj|
    canvas.scale = new_adj.value
  end

  w.set_size_request(50, -1)
  hbox.pack_start(w, :expand  => false,
                     :fill    => false,
                     :padding => 0)
  w.show

  scrolled_win = Gtk::ScrolledWindow.new
  vbox.pack_start(scrolled_win, :expand  => true,
                                :fill    => true,
                                :padding => 0)
  scrolled_win.show

  # Create the canvas.
  canvas.set_size_request(600, 450)
  setup_canvas(canvas, units, units_name)

  canvas.set_bounds(0, 0, 1000, 1000)
  canvas.units = units
  canvas.anchor = :center
  canvas.show
  
  scrolled_win.add(canvas)

  vbox
end

window = Gtk::Window.new(:toplevel)
window.set_default_size(640, 600)
window.show
window.signal_connect('delete_event') { Gtk.main_quit }

notebook = Gtk::Notebook.new
notebook.show
window.add(notebook)

notebook.append_page(create_canvas(:pixel,  'pixels'),      Gtk::Label.new('Pixels'))
notebook.append_page(create_canvas(:points, 'points'),      Gtk::Label.new('Points'))
notebook.append_page(create_canvas(:inch,   'inches'),      Gtk::Label.new('Inches'))
notebook.append_page(create_canvas(:mm,     'millimeters'), Gtk::Label.new('Millimeters'))

Gtk.main
