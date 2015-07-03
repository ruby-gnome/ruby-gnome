#!/usr/bin/env ruby
=begin
  combobox_from_cellrender.rb - Ruby/GTK sample script
  Copyright (c) 2015 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
=end

require "gtk3"

window = Gtk::Window.new "Combobox with CellRender"

lstore = Gtk::ListStore.new(TrueClass, String)

(1..5).each do |i|
  lstore.append.set_values([false, "item#{i}"])
end

combobox = Gtk::ComboBox.new(:model => lstore, :entry => true)

toggle = Gtk::CellRendererToggle.new

toggle.signal_connect "toggled" do |widget, path|
  puts widget.class
  puts path.class
end

combobox.pack_start(toggle, true)
combobox.add_attribute(toggle, "active", 0)
combobox.set_entry_text_column(1)

window.add combobox

combobox.signal_connect("changed") do |widget|
  list = widget.model
  list.each do |_model, _path, iter|
    iter[0] = false
  end
  iter = widget.active_iter
  puts iter.inspect
  puts "#{iter[0]} #{iter[1]}"

  iter[0] = true
end

window.signal_connect("destroy") { Gtk.main_quit }

window.show_all
Gtk.main
