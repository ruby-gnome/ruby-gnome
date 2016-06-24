#!/usr/bin/env ruby
=begin
  icons-theme-viewer.rb - Ruby/GTK sample script.

  Copyright (c) 2016 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
=end

require "gtk3"

def fill_model(icons)
  model = Gtk::ListStore.new(String, GdkPixbuf::Pixbuf)
  icons.each do |icon|
    pixbuf = Gtk::IconTheme.default.load_icon(icon, 32, 0)
    iter = model.append
    iter[0] = icon
    iter[1] = pixbuf
  end
  model
end

def gen_icon_view(pattern, context = nil)
  icon_theme = Gtk::IconTheme.default
  icons = icon_theme.icons(context).grep(/#{pattern}/)
  model = fill_model(icons)
  icon_view = Gtk::IconView.new(:model => model)
  icon_view.text_column = 0
  icon_view.pixbuf_column = 1
  icon_view
end

window = Gtk::Window.new("View all your icons")
window.set_default_size(700, 700)
window.signal_connect("delete-event") { Gtk.main_quit }

icon_view = gen_icon_view("application")
sw = Gtk::ScrolledWindow.new(nil, nil)
sw.add(icon_view)

entry = Gtk::Entry.new
entry.buffer.text = "application"
entry.tooltip_text = "Use a pattern to filter icons"
entry.set_icon_from_icon_name(:secondary, "edit-clear")
entry.set_icon_tooltip_text(:secondary, "Reset pattern")

entry.signal_connect "icon-release" do |widget, position|
  widget.buffer.text = "" if position == :secondary
end

entry.signal_connect "activate" do |widget|
  sw.remove(icon_view)
  pattern = widget.buffer.text
  icon_view = gen_icon_view(pattern)
  sw.add(icon_view)
  window.show_all
end

box = Gtk::Box.new(:vertical, 0)
box.pack_start(entry)
box.pack_start(sw, :expand => true, :fill => true)

window.add(box)
window.show_all

Gtk.main
