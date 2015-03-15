#!/usr/bin/env ruby
=begin
  dialog.rb - Ruby/GTK2 sample script.

  Copyright (c) 2002-2006 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
=end

require "gtk3"

window = Gtk::Window.new("Gtk::Dialog sample")
button = Gtk::Button.new(:label => "Create Dialog")
button.signal_connect("clicked") do
  dialog = Gtk::Dialog.new
  dialog.title = "Gtk::Dialog Sample"
  dialog.transient_for = window
  dialog.set_default_size(300, 300)
  dialog.child.add(Gtk::Label.new("Gtk::Dialog Sample"))


  dialog.add_button("OK", Gtk::ResponseType::OK)
  dialog.add_button(Gtk::Stock::CANCEL, Gtk::ResponseType::CANCEL)
  dialog.add_button(Gtk::Stock::CLOSE, Gtk::ResponseType::CLOSE)
  dialog.set_default_response(Gtk::ResponseType::CANCEL)

  dialog.signal_connect("response") do |widget, response|
    case response
    when Gtk::ResponseType::OK
      p "OK"
    when Gtk::ResponseType::CANCEL
      p "Cancel"
    when Gtk::ResponseType::CLOSE
      p "Close"
      dialog.destroy
    end
  end
  dialog.show_all
end

window.add(button)
window.signal_connect("destroy") {Gtk.main_quit}
window.show_all
Gtk.main
