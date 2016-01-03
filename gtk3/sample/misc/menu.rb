#!/usr/bin/env ruby
=begin
  menu.rb - Ruby/GTK2 sample script.

  Copyright (c) 2002-2015 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
=end

require "gtk3"

def create_menu(depth)
  return nil if depth < 1

  menu = Gtk::Menu.new
  group = nil

  (0..4).each do |i|
    buf = format("item %2d - %d", depth, i + 1)
    menuitem = Gtk::RadioMenuItem.new(group, buf)
    group = menuitem.group
    menu.append(menuitem)

    menuitem.set_submenu create_menu(depth - 1) if depth > 1
  end
  menu
end

def create_option_menu
  lstore = Gtk::ListStore.new(TrueClass, String)
  (1..5).each do |i|
    lstore.append.set_values([false, "item #{i}"])
  end

  combobox = Gtk::ComboBox.new(:model => lstore, :entry => true)

  toggle = Gtk::CellRendererToggle.new

  combobox.pack_start(toggle, true)
  combobox.add_attribute(toggle, "active", 0)
  combobox.set_entry_text_column(1)

  combobox.signal_connect("changed") do |widget|
    list = widget.model
    list.each do |model, _path, iter|
      model.set_value(iter, 0, false)
    end
    iter = widget.active_iter
    list.set_value(iter, 0, true)
  end

  combobox
end

window = Gtk::Window.new("menus")
window.signal_connect("destroy") do
  Gtk.main_quit
end
window.border_width = 0

box1 = Gtk::Box.new(:vertical, 0)
window.add(box1)

menubar = Gtk::MenuBar.new
box1.pack_start(menubar, :expand => false, :fill => true, :padding => 0)

menu = create_menu(2)
menuitem = Gtk::MenuItem.new(:label => "test\nline2")
menuitem.set_submenu(menu)
menubar.append(menuitem)

menuitem = Gtk::MenuItem.new(:label => "foo")
menuitem.set_submenu(create_menu(3))
menubar.append(menuitem)

menuitem = Gtk::MenuItem.new(:label => "bar")
menuitem.set_submenu(create_menu(4))
menubar.append(menuitem)

box2 = Gtk::Box.new(:vertical, 10)
box2.border_width = 10
box1.pack_start(box2, :expand => true, :fill => true, :padding => 0)

optionmenu = create_option_menu
box2.pack_start(optionmenu, :expand => true, :fill => true, :padding => 0)

separator = Gtk::Separator.new(:horizontal)
box1.pack_start(separator, :expand => false, :fill => true, :padding => 0)

box2 = Gtk::Box.new(:horizontal, 10)
box2.border_width = 10
box1.pack_start(box2, :expand => false, :fill => true, :padding => 0)

button = Gtk::Button.new(:label => "close")
button.signal_connect("clicked") do
  window.destroy
end
box2.pack_start(button, :expand => true, :fill => true, :padding => 0)

window.show_all

Gtk.main
