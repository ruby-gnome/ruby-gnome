# Copyright (c) 2015 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
=  Menus

 There are several widgets involved in displaying menus. The
 GtkMenuBar widget is a menu bar, which normally appears horizontally
 at the top of an application, but can also be layed out vertically.
 The GtkMenu widget is the actual menu that pops up. Both GtkMenuBar
 and GtkMenu are subclasses of GtkMenuShell; a GtkMenuShell contains
 menu items (GtkMenuItem). Each menu item contains text and/or images
 and can be selected by the user.

 There are several kinds of menu item, including plain GtkMenuItem,
 GtkCheckMenuItem which can be checked/unchecked, GtkRadioMenuItem
 which is a check menu item that's in a mutually exclusive group,
 GtkSeparatorMenuItem which is a separator bar, GtkTearoffMenuItem
 which allows a GtkMenu to be torn off, and GtkImageMenuItem which
 can place a GtkImage or other widget next to the menu text.

 A GtkMenuItem can have a submenu, which is simply a GtkMenu to pop
 up when the menu item is selected. Typically, all menu items in a menu bar
 have submenus.
=end
module MenusDemo
  def self.run_demo(main_window)
    window = Gtk::Window.new(:toplevel)
    window.screen = main_window.screen
    window.title = "Menus"

    accel_group = Gtk::AccelGroup.new
    window.add_accel_group(accel_group)
    window.border_width = 0

    box = Gtk::Box.new(:horizontal, 0)
    window.add(box)
    box.show

    box1 = Gtk::Box.new(:horizontal, 0)
    box.add(box1)
    box1.show

    menubar = Gtk::MenuBar.new
    menubar.expand = true
    box1.pack_start(menubar, :expand => false, :fill => true, :padding => 0)
    menubar.show

    menu = create_menu(2)

    menuitem = Gtk::MenuItem.new(:label => "test\nline2")
    menuitem.submenu = menu
    menubar.append(menuitem)
    menuitem.show

    menuitem = Gtk::MenuItem.new(:label => "foo")
    menuitem.submenu = create_menu(3)
    menubar.append(menuitem)
    menuitem.show

    menuitem = Gtk::MenuItem.new(:label => "bar")
    menuitem.submenu = create_menu(4)
    menubar.append(menuitem)
    menuitem.show

    box2 = Gtk::Box.new(:vertical, 10)
    box2.border_width = 10
    box1.pack_start(box2, :expand => false, :fill => true, :padding => 0)
    box2.show

    button = Gtk::Button.new(:label => "Flip")
    button.signal_connect("clicked") do |_widget|
      parent = menubar.parent
      orientation = parent.orientation.to_i
      parent.orientation = 1 - orientation

      if orientation == Gtk::Orientation::VERTICAL
        menubar.set_property("pack-direction", Gtk::PackDirection::TTB)
      else
        menubar.set_property("pack-direction", Gtk::PackDirection::LTR)
      end
    end
    box2.pack_start(button, :expand => true, :fill => true, :padding => 0)
    button.show
    button = Gtk::Button.new(:label => "Close")
    button.signal_connect("clicked") do |_widget|
      window.destroy
    end
    box2.pack_start(button, :expand => true, :fill => true, :padding => 0)
    button.can_default = true
    button.grab_default
    button.show

    if !window.visible?
      window.show_all
    else
      window.destroy
    end
    window
  end

  def self.create_menu(depth)
    return nil if depth < 1

    menu = Gtk::Menu.new
    last_item = nil
    (0..5).each do |i|
      j = i + 1
      label = "item #{depth} - #{j}"
      menu_item = Gtk::RadioMenuItem.new(nil, label)
      menu_item.join_group(last_item) if last_item
      last_item = menu_item
      menu.append(menu_item)
      menu_item.show
      menu_item.sensitive = false if i == 3
      menu_item.submenu = create_menu(depth - 1)
    end

    menu
  end
end
