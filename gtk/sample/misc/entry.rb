#!/usr/local/bin/ruby
=begin
  entry.rb - Ruby/GTK sample script.

  Copyright (c) 2002 Ruby-GNOME2 Project
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: entry.rb,v 1.2 2002/11/05 10:39:11 mutoh Exp $
=end

require 'gtk2'

Gtk.init

Gtk::RC::parse_string <<EOS
style "default"
{
  fontset = "-adobe-helvetica-medium-r-normal--*-120-*-*-*-*-*-*,*"
}
widget_class "*" style "default"
EOS

window = Gtk::Window.new
window.set_title("entry")
window.border_width = 0

box1 = Gtk::VBox.new(false, 0)
window.add(box1)

box2 = Gtk::VBox.new(false, 10)
box2.border_width = 10
box1.pack_start(box2, true, true, 0)

entry = Gtk::Entry.new
entry.set_text("hello world")
box2.pack_start(entry, true, true, 0)

separator = Gtk::HSeparator.new
box1.pack_start(separator, false, true, 0)

box2 = Gtk::VBox.new(false, 10)
box2.border_width = 10
box1.pack_start(box2, false, true, 0)

button = Gtk::Button.new("close")
button.signal_connect("clicked") do
  window.destroy
  exit
end
box2.pack_start(button, true, true, 0)
button.set_flags(Gtk::Widget::CAN_DEFAULT);
button.grab_default

window.show_all

Gtk.main
