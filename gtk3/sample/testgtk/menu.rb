=begin header

  menu.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002-2005 Ruby-GNOME2 Project Team

  $Id: menu.rb,v 1.6 2005/07/18 17:13:32 mutoh Exp $

=end

require 'sample'

class MenuSample < SampleWindow
  def initialize
    super("menus")
    signal_connect("delete_event") do true end

    box = Gtk::VBox.new(false, 10)
    add(box)

    menubar = Gtk::MenuBar.new
    box.pack_start(menubar, false, true, 0)

    menu = create_menu(2, true)

    menuitem = Gtk::MenuItem.new("test\nline2")
    menuitem.set_submenu(menu)
    menubar.append(menuitem)
    
    menuitem = Gtk::MenuItem.new("foo")
    menuitem.set_submenu(create_menu(3, true))
    menubar.append(menuitem)

    menuitem = Gtk::MenuItem.new("bar")
    menuitem.set_submenu(create_menu(4, true))
    menuitem.right_justified = true
    menubar.append(menuitem)

    optionmenu = Gtk::OptionMenu.new
    optionmenu.set_menu(create_menu(1, false))
    optionmenu.history = 4
    box.add(optionmenu)

    box.add(Gtk::HSeparator.new)

    button = Gtk::Button.new("close")
    button.signal_connect("clicked"){destroy}

    box.add(button)

    button.set_flags(Gtk::Widget::CAN_DEFAULT)
    button.grab_default
  end
  private
  def create_menu(depth, tearoff)
    return nil if depth < 1

    menu = Gtk::Menu.new
    group = nil

    if tearoff then
      menuitem = Gtk::TearoffMenuItem.new
      menu.append(menuitem)
    end

    for i in 0..4
      j = i + 1
      buf = sprintf("item %2d - %d", depth, j)
      menuitem = Gtk::RadioMenuItem.new(group, buf)
      group = menuitem.group
      menu.append(menuitem)
      menuitem.set_sensitive(false) if i == 3
      submenu = create_menu(depth - 1, true)
      menuitem.set_submenu(submenu) unless submenu.nil?
    end

    menu
  end
end
