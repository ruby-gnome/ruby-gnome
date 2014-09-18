#!/usr/bin/env ruby
=begin
  dialog2.rb - Ruby/GTK2 sample script.

  Copyright (c) 2002-2006 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: dialog2.rb,v 1.7 2006/06/17 13:18:12 mutoh Exp $
=end

require 'gtk3'

window = Gtk::Window.new("Gtk::Dialog sample2")
button = Gtk::Button.new(:label => "Create Dialog")
button.signal_connect("clicked") do 
  dialog = Gtk::Dialog.new(:title =>"Gtk::Dialog Sample 2",
                           :parent => window,
                           :flags => Gtk::Dialog::Flags::MODAL,
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

window.add(button).show_all.signal_connect("destroy"){Gtk.main_quit}

Gtk.main


                         

