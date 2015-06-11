#!/usr/bin/env ruby
=begin
  radiobutton.rb - Ruby/GTK2 sample script.

  Copyright (c) 2002-2015 Ruby-GNOME2 Project Team 
  This program is licenced under the same licence as Ruby-GNOME2.
=end

require "gtk3"

window = Gtk::Window.new("Gtk::RadioButton sample")
window.signal_connect("destroy") { Gtk.main_quit }

main_vbox = Gtk::Box.new(:vertical)
window.add(main_vbox)

top_box = Gtk::Box.new(:vertical, 10)
top_box.border_width = 10
main_vbox.add(top_box)

button1 = Gtk::RadioButton.new(:label => "_button1", :use_underline => true)
button2 = Gtk::RadioButton.new(:member => button1, :label => "_button2", :use_underline => false)

top_box.add(button1)
top_box.add(button2)

main_vbox.add(Gtk::Separator.new(:horizontal))

bottom_box = Gtk::Box.new(:vertical, 10)
bottom_box.border_width = 10
main_vbox.add(bottom_box)

close = Gtk::Button.new(:label => "close")
close.signal_connect("clicked") do
  Gtk.main_quit
end

bottom_box.add(close)

close.can_default = true
close.grab_default

window.show_all

Gtk.main
