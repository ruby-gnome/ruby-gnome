#!/usr/bin/env ruby
=begin
  filechooser.rb - Ruby/GTK2 sample script.

  Copyright (c) 2004-2020 Ruby-GNOME Project Team
  This program is licenced under the same licence as Ruby-GNOME.
=end

require "gtk3"

dialog = Gtk::FileChooserDialog.new(:title => "Gtk::FileChooser sample",
                                    :action => :open,
                                    :buttons => [[Gtk::Stock::OPEN, :accept],
                                                 [Gtk::Stock::CANCEL, :cancel]])

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

if dialog.run == Gtk::ResponseType::ACCEPT
  puts "filename = #{dialog.filename}"
  puts "uri = #{dialog.uri}"
end
