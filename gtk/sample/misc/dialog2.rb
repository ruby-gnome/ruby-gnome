#!/usr/local/bin/ruby
=begin
  dialog2.rb - Ruby/GTK sample script.

  Copyright (c) 2002 Ruby-GNOME2 Project
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: dialog2.rb,v 1.1 2002/12/17 17:34:44 mutoh Exp $
=end

require 'gtk2'

Gtk.init

window = Gtk::Window.new
button = Gtk::Button.new("Create Dialog")
button.signal_connect("clicked") do 
  dialog = Gtk::Dialog.new("Gtk::Dialog Sample 2", window, 
                           Gtk::Dialog::MODAL|Gtk::Dialog::NO_SEPARATOR,
                           [[Gtk::Stock::OK, Gtk::Dialog::RESPONSE_OK],
                             [Gtk::Stock::CANCEL, Gtk::Dialog::RESPONSE_CANCEL]
                           ])
  dialog.vbox.add(Gtk::Label.new("Gtk::Dialog Sample 2"))
  dialog.set_default_size(300, 300)
  dialog.vbox.show_all
  result = dialog.run
  case result
  when Gtk::Dialog::RESPONSE_OK
    p "OK"
  when Gtk::Dialog::RESPONSE_CANCEL
    p "Cancel"
  end
  dialog.destroy
end

window.add(button).show_all

Gtk.main


                         

