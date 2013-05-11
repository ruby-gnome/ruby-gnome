#!/usr/bin/env ruby
#
# This sample code is a port of
# goocanvas/demo/scalability-demo.c. It is licensed
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

require "goocanvas-gi"

Goo.init

# the values behind the comments are the ones used in the C version
# but as with them it takes more than 20 sec on my computer for the window
# to show upm I decreased them a little
# NOTE: Because it was slow, I lowered it.
#N_GROUP_COLS = 15 #25
N_GROUP_COLS = 10 #25
#N_GROUP_ROWS = 10 #20
N_GROUP_ROWS = 5 #20
N_COLS = 10
N_ROWS = 10

PADDING = 10

ONLY_ONE = false

def setup_canvas(canvas)
  root = canvas.root_item
  root.font = 'Sans 8'

  item_width = 400
  item_height = 19

  cell_width = item_width + PADDING * 2
  cell_height = item_height + PADDING * 2

  group_width = N_COLS * cell_width
  group_height = N_ROWS * cell_height

  total_width = N_GROUP_COLS * group_width
  total_height = N_GROUP_ROWS * group_height

  # We use -ve offsets to test if -ve coords are handled correctly.
  left_offset = -total_width / 2
  top_offset = -total_height / 2

  style = Goo::CanvasStyle.new
  color = Gdk::Color.parse("mediumseagreen")
  pattern = Cairo::SolidPattern.new(color.red / 65535.0, color.green / 65535.0, color.blue / 65535.0)
  # NOTE: "Goo::CanvasStyle#fill_pattern=" doesn't work. (undefined method)
  # Substitution is "Goo::CanvasRect#fill_color=".
  #style.fill_pattern = pattern

  style2 = Goo::CanvasStyle.new
  color = Gdk::Color.parse("steelblue")
  pattern = Cairo::SolidPattern.new(color.red / 65535.0, color.green / 65535.0, color.blue / 65535.0)
  # NOTE: "Goo::CanvasStyle#fill_pattern=" doesn't work. (undefined method)
  # Substitution is "Goo::CanvasRect#fill_color=".
  #style2.fill_pattern = pattern

  total_items = 0

  N_GROUP_COLS.times do |group_i|
    N_GROUP_ROWS.times do |group_j|
      group_x = left_offset + (group_i * group_width);
      group_y = top_offset + (group_j * group_height);

      group = Goo::CanvasGroup.new(:parent => root)
      total_items += 1
      group.translate(group_x, group_y)

      N_COLS.times do |i|
        N_ROWS.times do |j|
          item_x = (i * cell_width) + PADDING
          item_y = (j * cell_height) + PADDING
          rotation = i % 10 * 2
          rotation_x = item_x + item_width / 2
          rotation_y = item_y + item_height / 2

          current_id = "#{group_x + item_x}, #{group_y + item_y}"

          item = Goo::CanvasRect.new(:parent => group,
                                     :x      => item_x,
                                     :y      => item_y,
                                     :width  => item_width,
                                     :height => item_height)
          # NOTE: "Goo::CanvasStyle#fill_pattern=" doesn't work. (undefined method)
          # "Goo::CanvasRect#fill_color=" is substitution.
          item.fill_color = (j % 2) == 1 ? "mediumseagreen" : "steelblue"
          #item.style = (j % 2) == 1 ? style : style2
          item.rotate(rotation, rotation_x, rotation_y)

          item.signal_connect('motion_notify_event') do
            puts "#{current_id} item received 'motion-notify' signal"
          end

          item = Goo::CanvasText.new(:parent => group,
                                     :text   => current_id,
                                     :x      => item_x + item_width / 2,
                                     :y      => item_y + item_height / 2,
                                     :width  => -1,
                                     :anchor => Goo::CanvasAnchorType::CENTER)
          item.rotate(rotation, rotation_x, rotation_y)

          total_items += 2
          break if ONLY_ONE
        end
        break if ONLY_ONE
      end
      break if ONLY_ONE
    end
    break if ONLY_ONE
  end

  puts "Total items: #{total_items}"
  [ total_width, total_height, left_offset, top_offset ]
end

def create_canvas
  # Create the canvas.
  canvas = Goo::Canvas.new
  canvas.set_size_request(600, 450)

  start = Time.new
  total_width, total_height, left_offset, top_offset = setup_canvas(canvas)
  puts "Create Canvas Time Used: #{Time.new - start}"

  start = Time.new
  canvas.set_bounds(left_offset, top_offset, left_offset + total_width, top_offset + total_height)
  canvas.show

  first_time = true
  # NOTE: "expose_event" was removed.
  canvas.signal_connect("draw") do
    if first_time
      puts "Update Canvas Time Used: #{Time.new - start}"
      first_time = false
    end
    false
  end

  canvas
end

window = Gtk::Window.new(:toplevel)
window.set_default_size(640, 600)
window.show
window.signal_connect('delete_event') { Gtk.main_quit }

scrolled_win = Gtk::ScrolledWindow.new
scrolled_win.show
window.add(scrolled_win)

GC.disable
canvas = create_canvas
scrolled_win.add(canvas)
GC.enable

Gtk.main
