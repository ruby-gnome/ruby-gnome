#!/usr/bin/env ruby
=begin
  combo_check.rb - Ruby/GTK sample script.

  Copyright (c) 2002-2015 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
=end

require "gtk3"

window = Gtk::Window.new("entry")
window.border_width = 0
window.signal_connect("destroy") do
  Gtk.main_quit
end

box1 = Gtk::Box.new(:vertical)
window.add(box1)

box2 = Gtk::Box.new(:vertical, 10)
box2.border_width = 10
box1.pack_start(box2)

entry = Gtk::Entry.new
entry.text = "hello world"
entry.set_editable(false)
entry.select_region(0, -1)
box2.pack_start(entry)

## Gtk::ComboBoxText is simpler than Gtk::ComboBox, but has less functions.
## Gtk::Combo has been deprecated.
cb = Gtk::ComboBoxText.new(:entry => true)
cb.append_text("item0")
cb.append_text("item1 item1")
cb.append_text("item2 item2 item2")
cb.append_text("item3 item3 item3 item3")
cb.append_text("item4 item4 item4 item4 item4")
cb.append_text("item5 item5 item5 item5 item5 item5")
cb.append_text("item6 item6 item6 item6 item6")
cb.append_text("item7 item7 item7 item7")
cb.append_text("item8 item8 item8")
cb.append_text("item9 item9")
cb.child.text = "hello world"
cb.child.select_region(0, -1)
cb.child.set_editable(false)
box2.pack_start(cb, :expand => true, :fill => true, :padding => 0)

check = Gtk::CheckButton.new("Editable")
box2.pack_start(check, :expand => false, :fill => true, :padding => 0)
check.signal_connect("toggled") do
  cb.child.set_editable(check.active?)
  entry.set_editable(check.active?)
end
check.set_state_flags(Gtk::StateFlags::NORMAL, true)
## Run this code to see all of GtkStateFlags ##
#  Gtk::StateFlags.constants.each do |i|
#    p i
#  end

box1.pack_start(Gtk::Separator.new(:horizontal))

box2 = Gtk::Box.new(:vertical, 10)
box2.border_width = 10
box1.pack_start(box2)

button = Gtk::Button.new(:label => "close")
button.signal_connect("clicked") do
  Gtk.main_quit
end
box2.pack_start(button)

button.can_default = true
button.grab_default

window.show_all

Gtk.main
