#!/usr/local/bin/ruby
=begin
  combo_check.rb - Ruby/GTK sample script.

  Copyright (c) 2002,2003 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: combo_check.rb,v 1.6 2003/04/01 16:26:32 mutoh Exp $
=end

require 'gtk2'

Gtk.init

window = Gtk::Window.new
window.title = "entry"
window.border_width = 0

box1 = Gtk::VBox.new(false, 0)
window.add(box1)
box1.show

box2 = Gtk::VBox.new(false, 10)
box2.border_width = 10
box1.pack_start(box2, true, true, 0)
box2.show

entry = Gtk::Entry.new
entry.set_text("hello world")
entry.select_region(0, -1)
box2.pack_start(entry, true, true, 0)
entry.show

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
cb.show

check = Gtk::CheckButton.new("Editable")
box2.pack_start(check, false, true, 0)
check.signal_connect("toggled") do
  cb.entry.set_editable(check.active?)
  entry.set_editable(check.active?)
end
check.set_state(0)
check.show

separator = Gtk::HSeparator.new
box1.pack_start(separator, false, true, 0)
separator.show

box2 = Gtk::VBox.new(false, 10)
box2.border_width = 10
box1.pack_start(box2, false, true, 0)
box2.show

button = Gtk::Button.new("close")
button.signal_connect("clicked") do
	Gtk.main_quit
end
box2.pack_start(button, true, true, 0)
button.set_flags(Gtk::Widget::CAN_DEFAULT)
button.grab_default
window.show_all

Gtk::main
