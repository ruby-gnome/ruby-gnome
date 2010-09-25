#!/usr/bin/env ruby
=begin
  combo_check.rb - Ruby/GTK sample script.

  Copyright (c) 2002-2006 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: combo_check.rb,v 1.9 2006/06/17 13:18:12 mutoh Exp $
=end

require 'gtk2'

window = Gtk::Window.new("entry")
window.border_width = 0

box1 = Gtk::VBox.new
window.add(box1)

box2 = Gtk::VBox.new(false, 10)
box2.border_width = 10
box1.pack_start(box2)

entry = Gtk::Entry.new
entry.text = "hello world"
entry.select_region(0, -1)
box2.pack_start(entry)

#
# Gtk::Combo has been deprecated. Use Gtk::ComboBox instead.
#
cb = Gtk::Combo.new
cb.set_popdown_strings(["item0",
			 "item1 item1",
			 "item2 item2 item2",
			 "item3 item3 item3 item3",
			 "item4 item4 item4 item4 item4",
			 "item5 item5 item5 item5 item5 item5",
			 "item6 item6 item6 item6 item6",
			 "item7 item7 item7 item7",
			 "item8 item8 item8",
			 "item9 item9"])
cb.entry.set_text("hello world")
cb.entry.select_region(0, -1)
box2.pack_start(cb, true, true, 0)

check = Gtk::CheckButton.new("Editable")
box2.pack_start(check, false, true, 0)
check.signal_connect("toggled") do
  cb.entry.set_editable(check.active?)
  entry.set_editable(check.active?)
end
check.state = 0

box1.pack_start(Gtk::HSeparator.new)

box2 = Gtk::VBox.new(false, 10)
box2.border_width = 10
box1.pack_start(box2)

button = Gtk::Button.new("close")
button.signal_connect("clicked") do
  Gtk.main_quit
end
box2.pack_start(button)

button.can_default = true
button.grab_default

window.show_all

Gtk.main
