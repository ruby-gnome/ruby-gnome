#!/usr/bin/env ruby
#
# This sample code is a port of clutter-gtk/examples/gtk-clutter-test-scroll.c.
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

N_WIDGETS     = 5
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

vbox.add(clutter)

stage = clutter.stage

button = Gtk::Button.new(:stock_id => Gtk::Stock::QUIT)
button.signal_connect("clicked") do |_button|
  window.destroy
end
vbox.pack_start(button, :expand => false, :fill => false, :padding => 0)

# and its background color
stage.background_color = Clutter::Color.new(0x61, 0x64, 0x8c, 0xff)

widgets = []

# create a new group to hold multiple actors in a group
group = Clutter::Group.new

create_gtk_actor = lambda do
  gtk_actor = ClutterGtk::Actor.new
  bin = gtk_actor.widget

  scroll = Gtk::ScrolledWindow.new
  scroll.set_policy(:never, :automatic)
  bin.add(scroll)

  vbox = Gtk::Box.new(:vertical, 6)
  scroll.add_with_viewport(vbox)

  button = Gtk::Button.new(:label => "A Button")
  vbox.pack_start(button, :expand => false, :fill => false, :padding => 0)

  button.signal_connect("clicked") do |_button|
    puts("button clicked")
    label = Gtk::Label.new("A new label")
    label.show
    vbox.pack_start(label, :expand => false, :fill => false, :padding => 0)
  end

  6.times do |i|
    button = Gtk::CheckButton.new("Another button")
    vbox.pack_start(button, :expand => false, :fill => false, :padding => 0)
  end

  entry = Gtk::Entry.new
  vbox.pack_start(entry, :expand => false, :fill => false, :padding => 0)

  bin.show_all

  gtk_actor
end

N_WIDGETS.times do |i|
  widget = create_gtk_actor.call
  widgets[i] = widget

  # Place around a circle
  w = widgets.first.width
  h = widgets.first.height

  x = WINDOW_WIDTH / 2  +
    RADIUS * Math.cos(i * 2 * Math::PI / (N_WIDGETS)) - w / 2
  y = WINDOW_HEIGHT / 2 +
    RADIUS * Math.sin(i * 2 * Math::PI / (N_WIDGETS)) - h / 2

  widget.set_position(x, y)

  # Add to our group group
  group.add_actor(widget)
end

# Add the group to the stage
stage.add_actor(group)
stage.signal_connect("button-press-event") do |_stage, event|
  x, y = event.coords

  actor = _stage.get_actor_at_pos(:all, x, y)
  puts("click at #{x}, #{y} -> #{actor.gtype.name}:#{actor}")
  if !actor.is_a?(Clutter::Stage) and !actor.is_a?(Clutter::Group)
    actor.hide
  end
end
stage.signal_connect("key-release-event") do |_sage, event|
  unichar = [event.key_unicode].pack("U*")
  puts("*** key press event (key:#{unichar}) ***")

  if event.key_symbol == Clutter::Keys::KEY_q
    Gtk.main_quit
  end
end

window.show_all

# Only show the actors after parent show otherwise it will just be
# unrealized when the clutter foreign window is set. widget_show
# will call show on the stage.
group.show_all

# Create a timeline to manage animation
timeline = Clutter::Timeline.new(6000)
timeline.loop = true

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

Gtk.main
