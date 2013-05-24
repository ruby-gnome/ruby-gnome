#!/usr/bin/env ruby
#
# This sample code is a port of clutter-gtk/examples/gtk-clutter-window-test2.c.
# It is licensed under the terms of the GNU Lesser General Public
# License, version 2.1 or (at your option) later.
#
# The original header:
#   (c) 2009, Collabora Ltd.
#
#   Written by Davyd Madeley <davyd.madeley@collabora.co.uk>
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

require "clutter-gtk"
require "gdk_pixbuf2"

window = ClutterGtk::Window.new

table = Gtk::Grid.new

table.hexpand = true
table.vexpand = true

add_button = lambda do |stock_id, row|
  button = Gtk::Button.new(:stock_id => stock_id)
  table.insert_row(row)
  table.insert_column(row)
  table.attach(button, row, row, 1, 1)

  button.signal_connect("clicked") do |_button|
    puts("button clicked: #{stock_id}")

    toplevel = _button.toplevel
    if toplevel.is_a?(ClutterGtk::Window)
      # create a texture from the button image
      texture = ClutterGtk::Texture.new
      image = _button.image
      size = image.icon_size
      texture.set_from_stock(_button, stock_id.to_s, size)

      # position the texture on top of the existing icon
      stage = toplevel.stage
      stage.add_actor(texture)

      allocation = image.allocation

      # replace the icon itself
      blank = Gdk::Pixbuf.new(:colorspace => :rgb,
                              :has_alpha =>  true,
                              :bits_per_sample => 8,
                              :width => allocation.width,
                              :height => allocation.height)
      blank.fill!(0x00000000)
      image.pixbuf = blank

      # animate a fall due to gravity
      toplevel_allocation = toplevel.allocation
      texture.save_easing_state do
        texture.easing_mode = :ease_in_quad
        texture.easing_duration = 200
        texture.set_final_state("x", allocation.x.to_f)
        texture.set_final_state("y", allocation.y.to_f)
        texture.y = toplevel_allocation.height.to_f
      end

      first_completed_id = texture.signal_connect_after("transitions-completed") do
        image_allocation = image.allocation

        # do the second animation, have the icon grow out from the middle of the
        # button
        texture.save_easing_state do
          texture.easing_mode = :ease_out_sine
          texture.easing_duration = 100
          texture.set_final_state("x", image_allocation.x.to_f)
          texture.set_final_state("y", image_allocation.y.to_f)
          texture.set_final_state("scale-x", 0.0)
          texture.set_final_state("scale-y", 0.0)
          texture.scale_x = 1.0
          texture.scale_y = 1.0
          texture.set_final_state("scale-gravity", Clutter::Gravity::CENTER)
        end

        texture.signal_handler_disconnect(first_completed_id)

        second_completed_id = texture.signal_connect_after("transitions-completed") do
          # undo our changes
          puts("set stock = #{stock_id}, size = #{size}")
          image.icon_size = size
          image.stock = stock_id
          texture.signal_handler_disconnect(second_completed_id)
          texture.destroy
        end
      end
    end
  end
end

add_button.call(Gtk::Stock::OK,     0)
add_button.call(Gtk::Stock::CANCEL, 1)
add_button.call(Gtk::Stock::CLOSE,  2)
add_button.call(Gtk::Stock::ABOUT,  3)
add_button.call(Gtk::Stock::BOLD,   4)
add_button.call(Gtk::Stock::ITALIC, 5)

window.add(table)
window.show_all

# override the gtk-button-images setting, since we kind
# of rely on this to be true to actually show the stock
# icon falling off the button
settings = window.settings
settings.gtk_button_images = true

window.signal_connect("destroy") do
  Gtk.main_quit
end

Gtk.main
