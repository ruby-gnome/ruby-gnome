#!/usr/bin/env ruby
#
# This sample code is a port of
# clutter-gtk/examples/gtk-clutter-test.c. The image file used in
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

N_HANDS       = 4
WINDOW_WIDTH  = 400
WINDOW_HEIGHT = 400
RADIUS        = 150

fade_p        = false
fullscreen_p  = false

pixbuf = Gdk::Pixbuf.new(File.expand_path("redhand.png", File.dirname(__FILE__)))

window = Gtk::Window.new
window.set_default_size(WINDOW_WIDTH, WINDOW_HEIGHT)
window.title = "Clutter Embedding"
window.signal_connect("destroy") do
  Gtk.main_quit
end

vbox = Gtk::Grid.new
vbox.orientation = :vertical
vbox.hexpand = true
vbox.vexpand = true
window.add(vbox)

clutter = ClutterGtk::Embed.new
vbox.add(clutter)

stage = clutter.stage
stage.background_color = Clutter::Color.new(:sky_blue_light)

label = Gtk::Label.new("This is a label")
vbox.add(label)
label.hexpand = true

button = Gtk::Button.new(:label => "This is a button...clicky")
button.signal_connect("clicked") do |_button|
  fade_p = !fade_p
end
vbox.add(button)
button.hexpand = true

button = Gtk::Button.new(:label => "Fullscreen")
button.image = Gtk::Image.new(:stock => Gtk::Stock::FULLSCREEN, :size => :button)
button.signal_connect("clicked") do |_button|
  if fullscreen_p
    window.unfullscreen
    fullscreen_p = false
  else
    window.fullscreen
    fullscreen_p = true
  end
end
vbox.add(button)
button.hexpand = true

button = Gtk::Button.new(:stock_id => Gtk::Stock::QUIT)
button.signal_connect("clicked") do |_button|
  _button.destroy
end
vbox.add(button)
button.hexpand = true

group = Clutter::Actor.new
hands = []
N_HANDS.times do |i|
  if i.zero?
    hands[i] = ClutterGtk::Texture.new
    hands[i].set_from_pixbuf(pixbuf)
  else
    hands[i] = Clutter::Clone.new(hands[0])
  end

  # Place around a circle
  w = hands[0].width
  h = hands[0].height

  x = WINDOW_WIDTH / 2  + RADIUS * Math.cos(i * Math::PI / (N_HANDS / 2)) - w / 2
  y = WINDOW_HEIGHT / 2 + RADIUS * Math.sin(i * Math::PI / (N_HANDS / 2)) - h / 2

  hands[i].set_position(x, y)

  # Add to our group group
  group.add_child(hands[i])
end

# Add the group to the stage
stage.add_child(group)

constraint = Clutter::AlignConstraint.new(stage, :x_axis, 0.5)
group.add_constraint(constraint)
constraint = Clutter::AlignConstraint.new(stage, :y_axis, 0.5)
group.add_constraint(constraint)

stage.signal_connect("button-press-event") do |_stage, event|
  x, y = event.coords
  actor = _stage.get_actor_at_pos(:all, x, y)
  if actor.is_a?(ClutterGtk::Texture) or actor.is_a?(Clutter::Clone)
    actor.hide
  end

  Clutter::Event::STOP
end
stage.signal_connect("key-release-event") do |_stage, event|
  puts("*** key press event (key:#{[event.key_unicode].pack('U*')}) ***")
  case event.key_symbol
  when Clutter::Keys::KEY_q
      Gtk.main_quit
  when Clutter::Keys::KEY_r
    N_HANDS.times do |i|
      hands[i].show
    end
  end

  Clutter::Event::STOP
end

window.show_all

# Create a timeline to manage animation
timeline = Clutter::Timeline.new(6000)
timeline.repeat_count = -1

# fire a callback for frame change
timeline.signal_connect("new-frame") do |_timeline, msecs|
  rotation = _timeline.progress * 360.0
  group.set_rotation(:z_axis, rotation, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 0)
  N_HANDS.times do |i|
    hands[i].set_rotation(:z_axis,
                          -(6.0 * rotation),
                          hands[i].width / 2,
                          hands[i].height / 2,
                          0)
    if fade_p
      hands[i].opacity = (255 - (rotation % 255))
    end
  end
end

# and start it
timeline.start

Gtk.main
