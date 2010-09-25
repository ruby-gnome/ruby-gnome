#!/usr/bin/env ruby
=begin
  filechooser.rb - Ruby/GTK2 sample script.

  Copyright (c) 2004-2006 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: filechooser.rb,v 1.5 2006/06/17 13:18:12 mutoh Exp $
=end

require "gtk2"

if str = Gtk.check_version(2, 4, 0)
  puts "This sample requires GTK+ 2.4.0 or later"
  puts str
  exit
end

dialog =  Gtk::FileChooserDialog.new("Gtk::FileChooser sample", nil, 
				     Gtk::FileChooser::ACTION_OPEN,
				     "gnome-vfs",
				     [Gtk::Stock::OPEN, Gtk::Dialog::RESPONSE_ACCEPT],
				     [Gtk::Stock::CANCEL, Gtk::Dialog::RESPONSE_CANCEL]
				     )

extra_button = Gtk::Button.new("Extra button")
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

if dialog.run == Gtk::Dialog::RESPONSE_ACCEPT
  puts "filename = #{dialog.filename}"
  puts "uri = #{dialog.uri}"
end
