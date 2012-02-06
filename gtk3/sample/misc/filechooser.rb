#!/usr/bin/env ruby
=begin
  filechooser.rb - Ruby/GTK2 sample script.

  Copyright (c) 2004-2006 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: filechooser.rb,v 1.5 2006/06/17 13:18:12 mutoh Exp $
=end

require "gtk3"

dialog =  Gtk::FileChooserDialog.new(:title => "Gtk::FileChooser sample", 
				     :action => Gtk::FileChooser::Action::OPEN,                                 
				     :buttons => [[Gtk::Stock::OPEN, Gtk::Dialog::ResponseType::ACCEPT],
				     [Gtk::Stock::CANCEL, Gtk::Dialog::ResponseType::CANCEL]])

extra_button = Gtk::Button.new(:label => "Extra button")
extra_button.signal_connect("clicked") do
  puts "extra button is clicked"
end

dialog.extra_widget = extra_button

filter_rb = Gtk::FileFilter.new
filter_rb.name = "Ruby Scripts"
filter_rb.add_pattern("*.rb")
filter_rb.add_pattern("*.rbw")
dialog.add_filter(filter_rb)

filter_c = Gtk::FileFilter.new
filter_c.name = "C sources"
filter_c.add_pattern("*.[c|h]")
dialog.add_filter(filter_c)

dialog.add_shortcut_folder("/tmp")

if dialog.run == Gtk::Dialog::ResponseType::ACCEPT
  puts "filename = #{dialog.filename}"
  puts "uri = #{dialog.uri}"
end
