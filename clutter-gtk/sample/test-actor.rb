#!/usr/bin/env ruby
#
# This sample code is a port of clutter-gtk/examples/gtk-clutter-test-actor.c.
# It is licensed under the terms of the GNU Lesser General Public
# License, version 2.1 or (at your option) later.
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

MAX_N_WIDGETS = 4
WINDOW_WIDTH  = 400
WINDOW_HEIGHT = 400
RADIUS        = 80

do_rotate_p   = true

window = Gtk::Window.new
window.signal_connect("destroy") do
  Gtk.main_quit
end

vbox = Gtk::Box.new(:vertical, 6)
window.add(vbox)

clutter = ClutterGtk::Embed.new
clutter.set_size_request(WINDOW_WIDTH, WINDOW_HEIGHT)

vbox.pack_start(clutter, :expand => true, :fill => true, :padding => 0)

stage = clutter.stage

button = Gtk::Button.new(:stock_id => Gtk::Stock::QUIT)
button.signal_connect("clicked") do |_button|
  window.destroy
end
vbox.pack_end(button, :expand => false, :fill => false, :padding => 0)

stage.background_color = Clutter::Color.new(0x61, 0x64, 0x8c, 0xff)

widgets = []

# create a new group to hold multiple actors in a group
group = Clutter::Actor.new

create_gtk_actor = lambda do
  gtk_actor = ClutterGtk::Actor.new
  bin = gtk_actor.widget

  vbox = Gtk::Box.new(:vertical, 6)
  bin.add(vbox)

  button = Gtk::Button.new(:label => "A Button")
  vbox.pack_start(button, :expand => false, :fill => false, :padding => 0)

  button.signal_connect("clicked") do |_button|
    puts("button clicked")
    label = Gtk::Label.new("A new label")
    label.show
    vbox.pack_start(label, :expand => false, :fill => false, :padding => 0)
  end

  button = Gtk::CheckButton.new("Another button")
  vbox.pack_start(button, :expand => false, :fill => false, :padding => 0)

  entry = Gtk::Entry.new
  vbox.pack_start(entry, :expand => false, :fill => false, :padding => 0)

  bin.show_all

  gtk_actor
end

add_clutter_actor = lambda do |actor, container, i|
  container.add_child(actor)

  # Place around a circle
  w = widgets.first.width
  h = widgets.first.height

  x = WINDOW_WIDTH / 2  +
    RADIUS * Math.cos(i * 2 * Math::PI / (MAX_N_WIDGETS)) - w / 2
  y = WINDOW_HEIGHT / 2 +
    RADIUS * Math.sin(i * 2 * Math::PI / (MAX_N_WIDGETS)) - h / 2

  actor.set_position(x, y)
end

MAX_N_WIDGETS.times do |i|
  widget = create_gtk_actor.call
  widgets[i] = widget
  add_clutter_actor.call(widget, group, i)
end

# Add the group to the stage and center it
stage.add_child(group)
group.add_constraint(Clutter::AlignConstraint.new(stage, :x_axis, 0.5))
group.add_constraint(Clutter::AlignConstraint.new(stage, :y_axis, 0.5))

window.show_all

# Create a timeline to manage animation
timeline = Clutter::Timeline.new(6000)
timeline.repeat_count = -1

# fire a callback for frame change
timeline.signal_connect("new-frame") do |_timeline, m_secs|
  rotation = _timeline.progress * 360.0
  if do_rotate_p
    # Rotate everything clockwise about stage center
    group.set_rotation(:z_axis,
                       rotation,
                       WINDOW_WIDTH / 2,
                       WINDOW_HEIGHT / 2,
                       0)

    widgets.each do |widget|
      # rotate each widget around its center
      w = widget.width
      h = widget.height

      widget.set_rotation(:z_axis,
                          -(2 * rotation),
                          w / 2,
                          h / 2,
                          0)
      widget.opacity = 50 * Math.sin(2 * Math::PI * rotation / 360) + (255 - 50)
    end
  end
end

# and start it
timeline.start

GLib::Timeout.add_seconds(3) do
  if MAX_N_WIDGETS == widgets.size
    # Removing an item
    group.remove_child(widgets.pop)
  else
    # Adding an item
    widget = create_gtk_actor.call
    widgets << widget
    add_clutter_actor.call(widget, group, MAX_N_WIDGETS - 1)
  end
  keep_callback = true
  keep_callback
end

Gtk.main
