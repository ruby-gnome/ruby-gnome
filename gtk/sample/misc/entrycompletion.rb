#!/usr/bin/env ruby
=begin
  entrycompletion.rb - Ruby/GTK sample script.

  Copyright (c) 2004 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: entrycompletion.rb,v 1.2 2004/05/24 17:22:57 mutoh Exp $
=end

require 'gtk2'

Gtk.init

if str = Gtk.check_version(2, 4, 0)
  puts "This sample requires GTK+ 2.4.0 or later"
  puts str
  exit
end

window = Gtk::Window.new
window.set_default_size(200, 200)

entry = Gtk::Entry.new
completion = Gtk::EntryCompletion.new
entry.completion = completion

model = Gtk::ListStore.new(String)
["GNOME", "total", "totally"].each do |v|
  iter = model.append
  iter[0] = v
end

completion.model = model
completion.text_column = 0

window.add(entry).show_all

Gtk.main
