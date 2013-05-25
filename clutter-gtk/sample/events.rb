#!/usr/bin/env ruby
#
# This sample code is a port of
# clutter-gtk/examples/gtk-clutter-events.c. The image file used in
# this sample code is copied from
# clutter-gtk/examples/redhand.png. They are licensed under the terms
# of the GNU Lesser General Public License, version 2.1 or (at your
# option) later.
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

window = Gtk::Window.new
window.title = "Gtk-Clutter Interaction demo"
window.set_default_size(800, 600)
window.resizable = false
window.border_width = 12
window.signal_connect("destroy") do
  Gtk.main_quit
end

vbox = Gtk::Box.new(:vertical, 12)
window.add(vbox)

clutter_entry = nil
gtk_entry = Gtk::Entry.new
gtk_entry.text = "Enter some text"
vbox.pack_start(gtk_entry, :expand => false, :fill => false, :padding => 0)
gtk_entry.signal_connect("changed") do |editable|
  clutter_entry.text = editable.text
end

hbox = Gtk::Box.new(:horizontal, 12)
vbox.pack_start(hbox, :expand => true, :fill => true, :padding => 0)

# Set up clutter & create our stage
clutter_embed = ClutterGtk::Embed.new
hbox.pack_start(clutter_embed, :expand => true, :fill => true, :padding => 0)
clutter_embed.grab_focus
stage = clutter_embed.stage
clutter_embed.set_size_request(640, 480)
stage.signal_connect("captured-event") do |_stage, event|
  case event
  when Clutter::ButtonEvent
    puts("#{event.type.nick} captured at (#{event.x}, #{event.y})");
  when Clutter::CrossingEvent
    if event.source == _stage and not event.related.nil?
      if event.type == Clutter::EventType::ENTER
        stage_action = "Entering"
        actor_action = "leaving"
      else
        stage_action = "Leaving"
        actor_action = "entering"
      end
      puts("#{stage_action} the stage and #{actor_action} '#{event.related.name}'")
    end
  when Clutter::KeyEvent
    if event.type == Clutter::EventType::KEY_PRESS
      format = "the stage got a key press: '%s' (symbol: %d, unicode: 0x%x)"
      unicode = [event.key_unicode].pack("U")
      puts(format % [unicode, event.key_symbol, event.key_unicode])
    end
  end

  not Clutter::Event::STOP
end

clutter_embed.signal_connect("enter-notify-event") do |widget|
  puts("Entering widget '#{widget.class.name}'")
  false
end
clutter_embed.signal_connect("leave-notify-event") do |widget|
  puts("Leaving widget '#{widget.class.name}'")
end

# Create the main texture that the spin buttons manipulate
pixbuf = Gdk::Pixbuf.new(File.expand_path("redhand.png", File.dirname(__FILE__)));
hand = ClutterGtk::Texture.new
hand.from_pixbuf = pixbuf
stage.add_child(hand)
hand.anchor_point_from_gravity = :center
hand.set_position(stage.width / 2, stage.height / 2)
hand.reactive = true
hand.name = "Red Hand"
hand.signal_connect("button-press-event") do |actor, event|
  puts("Button press on hand ('#{actor.class.name}')")
  not Clutter::Event::STOP
end

# Setup the clutter entry
clutter_entry = Clutter::Text.new
clutter_entry.color = Clutter::Color.new(:black)
stage.add_child(clutter_entry)
clutter_entry.set_position(0, 0)
clutter_entry.set_size(500, 20)

# Create our adjustment widgets
vbox = Gtk::Box.new(:vertical, 6)
hbox.pack_start(vbox, :expand => false, :fill => false, :padding => 0)

box = Gtk::Box.new(:horizontal, 6)
vbox.pack_start(box, :expand => false, :fill => true, :padding => 0)
label = Gtk::Label.new("Rotate x-axis")
box.pack_start(label, :expand => true, :fill => true, :padding => 0)
button = Gtk::SpinButton.new(0, 360, 1)
box.pack_start(button, :expand => true, :fill => true, :padding => 0)
button.signal_connect("value-changed") do |_button|
  hand.set_rotation(:x_axis,
                    _button.value,
                    0,
                    hand.height / 2,
                    0)
end

box = Gtk::Box.new(:horizontal, 6)
vbox.pack_start(box, :expand => false, :fill => true, :padding => 0)
label = Gtk::Label.new("Rotate y-axis")
box.pack_start(label, :expand => true, :fill => true, :padding => 0)
button = Gtk::SpinButton.new(0, 360, 1)
box.pack_start(button, :expand => true, :fill => true, :padding => 0)
button.signal_connect("value-changed") do |_button|
  hand.set_rotation(:y_axis,
                    _button.value,
                    hand.width / 2,
                    0,
                    0)
end

box = Gtk::Box.new(:horizontal, 6)
vbox.pack_start(box, :expand => false, :fill => true, :padding => 0)
label = Gtk::Label.new("Rotate z-axis")
box.pack_start(label, :expand => true, :fill => true, :padding => 0)
button = Gtk::SpinButton.new(0, 360, 1)
box.pack_start(button, :expand => true, :fill => true, :padding => 0)
button.signal_connect("value-changed") do |_button|
  hand.set_rotation(:z_axis,
                    _button.value,
                    hand.width / 2,
                    hand.height / 2,
                    0)
end

box = Gtk::Box.new(:horizontal, 6)
vbox.pack_start(box, :expand => false, :fill => true, :padding => 0)
label = Gtk::Label.new("Adjust opacity")
box.pack_start(label, :expand => true, :fill => true, :padding => 0)
button = Gtk::SpinButton.new(0, 255, 1)
button.value = 255
box.pack_start(button, :expand => true, :fill => true, :padding => 0)
button.signal_connect("value-changed") do |_button|
  hand.opacity = _button.value
end

window.show_all

# Only show/show_all the stage after parent show. widget_show will call
# show on the stage.
stage.show

Gtk.main
