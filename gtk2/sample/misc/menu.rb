#!/usr/bin/env ruby
=begin
  menu.rb - Ruby/GTK2 sample script.

  Copyright (c) 2002-2006 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: menu.rb,v 1.8 2006/06/17 13:18:12 mutoh Exp $
=end

require 'gtk2'

def create_menu(depth)
  return nil if depth < 1
  
  menu = Gtk::Menu.new
  group = nil
  submenu = nil

  for i in 0..4
    buf = sprintf("item %2d - %d", depth, i + 1)
    menuitem = Gtk::RadioMenuItem.new(group, buf)
    group = menuitem.group
    menu.append(menuitem)
    if depth > 1
      menuitem.set_submenu create_menu(depth - 1)
    end
  end
  menu
end

window = Gtk::Window.new("menus")
window.signal_connect("destroy") do
  Gtk.main_quit
end
window.border_width = 0

box1 = Gtk::VBox.new(false, 0)
window.add(box1)

menubar = Gtk::MenuBar.new
box1.pack_start(menubar, false, true, 0)

menu = create_menu(2)
menuitem = Gtk::MenuItem.new("test\nline2")
menuitem.set_submenu(menu)
menubar.append(menuitem)

menuitem = Gtk::MenuItem.new("foo")
menuitem.set_submenu(create_menu(3))
menubar.append(menuitem)

menuitem = Gtk::MenuItem.new("bar")
menuitem.set_submenu(create_menu(4))
menubar.append(menuitem)

box2 = Gtk::VBox.new(false, 10)
box2.border_width = 10
box1.pack_start(box2, true, true, 0)

optionmenu = Gtk::OptionMenu.new
optionmenu.set_menu(create_menu(1))
optionmenu.set_history(4)
box2.pack_start(optionmenu, true, true, 0)

separator = Gtk::HSeparator.new
box1.pack_start(separator, false, true, 0)

box2 = Gtk::HBox.new(false, 10)
box2.border_width = 10
box1.pack_start(box2, false, true, 0)

button = Gtk::Button.new("close")
button.signal_connect("clicked") do
  window.destroy
end
box2.pack_start(button, true, true, 0)

window.show_all

Gtk.main
