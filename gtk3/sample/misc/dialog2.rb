#!/usr/bin/env ruby
=begin
  dialog2.rb - Ruby/GTK2 sample script.

  Copyright (c) 2002-2015 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
=end

require "gtk3"

window = Gtk::Window.new("Gtk::Dialog sample2")
button = Gtk::Button.new(:label => "Create Dialog")
button.signal_connect("clicked") do
  dialog = Gtk::Dialog.new(:title =>"Gtk::Dialog Sample 2",
                           :parent => window,
                           :flags => :modal,
                           :buttons => [[Gtk::Stock::OK, Gtk::ResponseType::OK],
                           [Gtk::Stock::CANCEL, Gtk::ResponseType::CANCEL]])
  dialog.child.add(Gtk::Label.new("Gtk::Dialog Sample 2"))
  dialog.set_default_size(300, 300)
  dialog.child.show_all
  result = dialog.run
  case result
  when Gtk::ResponseType::OK
    p "OK"
  when Gtk::ResponseType::CANCEL
    p "Cancel"
  end
  dialog.destroy
end

window.add(button)
window.signal_connect("destroy") {Gtk.main_quit}
window.show_all
Gtk.main
