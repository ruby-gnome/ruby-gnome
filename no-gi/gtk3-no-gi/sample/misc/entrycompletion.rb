#!/usr/bin/env ruby
=begin
  entrycompletion.rb - Ruby/GTK sample script.

  Copyright (c) 2004-2006 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  Enter "G", "t" on the entry box.

  $Id: entrycompletion.rb,v 1.4 2006/06/17 13:18:12 mutoh Exp $
=end

require 'gtk3'

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

window.add(entry).show_all.signal_connect("destroy"){Gtk.main_quit}

Gtk.main
