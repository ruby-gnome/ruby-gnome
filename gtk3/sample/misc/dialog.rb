#!/usr/bin/env ruby
=begin
  dialog.rb - Ruby/GTK2 sample script.

  Copyright (c) 2002-2006 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: dialog.rb,v 1.6 2006/06/17 13:18:12 mutoh Exp $
=end

require 'gtk3'

window = Gtk::Window.new("Gtk::Dialog sample")
button = Gtk::Button.new("Create Dialog")
button.signal_connect("clicked") do 
  dialog = Gtk::Dialog.new
  dialog.title = "Gtk::Dialog Sample"
  dialog.transient_for = window
  dialog.set_default_size(300, 300)
  dialog.vbox.add(Gtk::Label.new("Gtk::Dialog Sample"))


  dialog.add_button("OK", Gtk::Dialog::RESPONSE_OK)
  dialog.add_button(Gtk::Stock::CANCEL, Gtk::Dialog::RESPONSE_CANCEL)
  dialog.add_button(Gtk::Stock::CLOSE, Gtk::Dialog::RESPONSE_CLOSE)
  dialog.set_default_response(Gtk::Dialog::RESPONSE_CANCEL)

  dialog.signal_connect("response") do |widget, response|
    case response
    when Gtk::Dialog::RESPONSE_OK
      p "OK"
    when Gtk::Dialog::RESPONSE_CANCEL
      p "Cancel"
    when Gtk::Dialog::RESPONSE_CLOSE
      p "Close"
      dialog.destroy
    end
  end
  dialog.show_all
end

window.add(button).show_all.signal_connect("destroy"){Gtk.main_quit}

Gtk.main
