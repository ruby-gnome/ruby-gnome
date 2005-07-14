#!/usr/bin/env ruby
=begin
  expander.rb - Ruby/GTK sample script.

  Copyright (c) 2004,2005 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: expander.rb,v 1.2 2005/07/14 17:01:49 mutoh Exp $
=end

require 'gtk2'

Gtk.init

expander = Gtk::Expander.new("Expander Sample")
expander.expanded = true

button = Gtk::Button.new("Hide this button.")
button.signal_connect("clicked") do
  expander.expanded = false
end

button.set_size_request(200, 200)
expander.add(button)

Gtk::Window.new.add(expander).show_all.signal_connect("destroy"){Gtk.main_quit}

Gtk.main

