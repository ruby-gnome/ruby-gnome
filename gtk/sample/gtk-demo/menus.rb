# $Id: menus.rb,v 1.1 2003/02/25 15:07:22 kzys Exp $
=begin
= Menus

There are several widgets involved in displaying menus. The
GtkMenuBar widget is a horizontal menu bar, which normally appears
at the top of an application. The GtkMenu widget is the actual menu
that pops up. Both GtkMenuBar and GtkMenu are subclasses of
GtkMenuShell; a GtkMenuShell contains menu items
(GtkMenuItem). Each menu item contains text and/or images and can
be selected by the user.

There are several kinds of menu item, including plain GtkMenuItem,
GtkCheckMenuItem which can be checked/unchecked, GtkRadioMenuItem
which is a check menu item that's in a mutually exclusive group,
GtkSeparatorMenuItem which is a separator bar, GtkTearoffMenuItem
which allows a GtkMenu to be torn off, and GtkImageMenuItem which
can place a GtkImage or other widget next to the menu text.

A GtkMenuItem can have a submenu, which is simply a GtkMenu to pop
up when the menu item is selected. Typically, all menu items in a menu bar
have submenus.

The GtkOptionMenu widget is a button that pops up a GtkMenu when clicked.
It's used inside dialogs and such.

GtkItemFactory provides a higher-level interface for creating menu bars
and menus; while you can construct menus manually, most people don't
do that. There's a separate demo for GtkItemFactory.
=end
require 'common'

module Demo
  class Menus < BasicWindow
    def initialize
      super('menus')
      self.border_width = 0
      
      accel_group = Gtk::AccelGroup.new
      add_accel_group(accel_group)
      
      
      box1 = Gtk::VBox.new(false, 0)
      add(box1)
      
      menubar = Gtk::MenuBar.new
      box1.pack_start(menubar, false, true, 0)
      
      menu = create_menu(2, true)
      
      menuitem = Gtk::MenuItem.new("test\nline2")
      menuitem.submenu = menu
      menubar.append(menuitem)
      menuitem.show
      
      menuitem = Gtk::MenuItem.new('foo')
      menuitem.submenu = create_menu(3, true)
      menubar.append(menuitem)
      menuitem.show

      menuitem = Gtk::MenuItem.new('bar')
      menuitem.submenu = create_menu(4, true)
      menuitem.right_justified = true
      menubar.append(menuitem)
      menuitem.show
      
      box2 = Gtk::VBox.new(false, 10)
      box2.border_width = 10
      box1.pack_start(box2, true, true, 0)
      box2.show
      
      menu = create_menu(1, false)
      # menu.accel_group = accel_group

      menuitem = Gtk::SeparatorMenuItem.new
      menu.append(menuitem)
      menuitem.show
      
      menuitem = Gtk::CheckMenuItem.new('Accelerate Me')
      menu.append(menuitem)
      menuitem.show
      menuitem.add_accelerator('activate',
			       accel_group,
			       Gdk::Keyval::GDK_F1,
			       0,
			       Gtk::ACCEL_VISIBLE)
      menuitem = Gtk::CheckMenuItem.new('Accelerator Locked')
      menu.append(menuitem)
      menuitem.show
      menuitem.add_accelerator('activate',
			       accel_group,
			       Gdk::Keyval::GDK_F2,
			       0,
			       Gtk::ACCEL_VISIBLE | Gtk::ACCEL_LOCKED)
      menuitem = Gtk::CheckMenuItem.new('Accelerators Frozen')
      menu.append(menuitem)
      menuitem.show
      menuitem.add_accelerator('activate',
			       accel_group,
			       Gdk::Keyval::GDK_F2,
			       0,
			       Gtk::ACCEL_VISIBLE)
      menuitem.add_accelerator('activate',
			       accel_group,
			       Gdk::Keyval::GDK_F3,
			       0,
			       Gtk::ACCEL_VISIBLE)
      
      optionmenu = Gtk::OptionMenu.new
      optionmenu.menu = menu
      optionmenu.history = 3
      box2.pack_start(optionmenu, true, true, 0)
      optionmenu.show

      separator = Gtk::HSeparator.new
      box1.pack_start(separator, false, true, 0)
      separator.show

      box2 = Gtk::VBox.new(false, 10)
      box2.border_width = 10
      box1.pack_start(box2, false, true, 0)
      box2.show

      button = Gtk::Button.new('close')
      button.signal_connect('clicked') do
	quit
      end
      box2.pack_start(button, true, true, 0)
      button.flags = Gtk::Widget::CAN_DEFAULT
      button.grab_default
      button.show
    end


    def create_menu (depth, tearoff)
      if depth < 1
	return nil
      end

      menu = Gtk::Menu.new
      group = nil

      if tearoff
	menuitem = Gtk::TearoffMenuItem.new
	menu.append(menuitem)
	menuitem.show
      end

      5.times do |i|
	buf = sprintf('item %2d - %d', depth, i + 1)
	menuitem = Gtk::RadioMenuItem.new(buf)
	group = menuitem.group

	menu.append(menuitem)
	menuitem.show
	if i == 3
	  menuitem.sensitive = false
	end

	if submenu = create_menu(depth - 1, true)
	  menuitem.submenu = submenu
	end
      end

      menu.show
      return menu
    end
  end
end
