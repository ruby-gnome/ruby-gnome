#!/usr/bin/env ruby
=begin
  dialog2.rb - Ruby/GTK2 sample script.

  Copyright (c) 2002-2006 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: dialog2.rb,v 1.7 2006/06/17 13:18:12 mutoh Exp $
=end

require 'gtk2'

window = Gtk::Window.new("Gtk::Dialog sample2")
button = Gtk::Button.new("Create Dialog")
button.signal_connect("clicked") do 
  dialog = Gtk::Dialog.new("Gtk::Dialog Sample 2", window, 
                           Gtk::Dialog::MODAL|Gtk::Dialog::NO_SEPARATOR,
                           [Gtk::Stock::OK, Gtk::Dialog::RESPONSE_OK],
                           [Gtk::Stock::CANCEL, Gtk::Dialog::RESPONSE_CANCEL])
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

window.add(button).show_all.signal_connect("destroy"){Gtk.main_quit}

Gtk.main


                         

