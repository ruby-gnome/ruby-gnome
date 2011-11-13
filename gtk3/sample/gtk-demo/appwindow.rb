# Copyright (c) 2003-2005 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
# $Id: appwindow.rb,v 1.7 2005/03/05 15:01:16 mutoh Exp $
=begin
=  Application main window

Demonstrates a typical application window, with menubar, toolbar, statusbar.
=end

require 'common'

module Demo
  class AppWindow < BasicWindow
    def initialize
      register_stock_icons

      ## Create toplevel window
      super('Application Window')

      table = Gtk::Table.new(1, 4, false)
      add(table)

      ## Create the menubar
      accel_group = Gtk::AccelGroup.new
      add_accel_group(accel_group)

      item_factory = Gtk::ItemFactory.new(Gtk::ItemFactory::TYPE_MENU_BAR,
					  '<main>', accel_group)
      # create menu items
      menuitem_cb = proc do |data, widget|
	# TODO: Show item-factory's path to menu item
	dialog = Gtk::MessageDialog.new(self,
					Gtk::Dialog::DESTROY_WITH_PARENT,
					Gtk::MessageDialog::INFO,
					Gtk::MessageDialog::BUTTONS_CLOSE,
					"You selected or toggled the menu item \"#{Gtk::ItemFactory.path_from_widget(widget)}\"")
	
	# Close dialog on user response
	dialog.signal_connect('response') do |widget, data|
	  widget.destroy
	end
	
	dialog.show
      end

      menu_items = [
	["/_File"],
	["/_File/_New",
	  "<StockItem>", "<control>N", Gtk::Stock::NEW,  menuitem_cb],
	["/_File/_Open",
	  "<StockItem>", "<control>O", Gtk::Stock::OPEN, menuitem_cb],
	["/File/_Save",
	  "<StockItem>", "<control>S", Gtk::Stock::SAVE, menuitem_cb],
	["/File/Save _As...",
	  "<StockItem>", nil,          Gtk::Stock::SAVE, menuitem_cb],
	["/File/sep1",
	  "<Separator>", nil,          nil,              menuitem_cb],
	["/File/Quit",
	  "<StockItem>", "<control>Q", Gtk::Stock::QUIT, menuitem_cb],

	["/_Preferences"],
	["/_Preferences/_Color"],
	["/_Preferences/Color/_Red",
	  "<RadioItem>",            nil, nil, menuitem_cb],
	["/_Preferences/Color/_Green",
	  "/Preferences/Color/Red", nil, nil, menuitem_cb],
	["/_Preferences/Color/_Blue",
	  "/Preferences/Color/Red", nil, nil, menuitem_cb],
	
	["/Preferences/_Shape"],
	["/Preferences/Shape/_Square",
	  "<RadioItem>",                 nil, nil, menuitem_cb],
	["/Preferences/Shape/_Rectangle",
	  "/Preferences/Shape/Square",   nil, nil, menuitem_cb],
	["/Preferences/Shape/_Oval",
	  "/Preferences/Shape/Square",   nil, nil, menuitem_cb],

	# If you wanted this to be right justified you would use
	# "<LastBranch>", not "<Branch>".  Right justified help menu
	# items are generally considered a bad idea now days.
	["/_Help"],
	["/Help/_About", "<Item>", nil, nil, Proc.new{about_cb}],
      ]
      item_factory.create_items(menu_items)

      table.attach(item_factory.get_widget('<main>'),
		   # X direction            # Y direction
		   0, 1,                    0, 1,
		   Gtk::EXPAND | Gtk::FILL, 0,
		   0,                       0)

      ## Create the toolbar
      toolbar = Gtk::Toolbar.new
      # toolbar.set_toolbar_style(Gtk::Toolbar::BOTH)
      toolbar.append(Gtk::Stock::OPEN,
		     "This is a demo button with an 'open' icon"
		     ) do toolbar_cb end
      toolbar.append(Gtk::Stock::QUIT,
		     "This is a demo button with an 'quit' icon"
		     ) do toolbar_cb end
      toolbar.append_space
      toolbar.append(:demo_gtk_logo,
		     "This is a demo button with an 'gtk' icon"
		     ) do toolbar_cb end
      table.attach(toolbar,
		   # X direction            # Y direction
		   0, 1,                    1, 2,
		   Gtk::EXPAND | Gtk::FILL, 0,
		   0,                       0)

      ## Create document
      sw = Gtk::ScrolledWindow.new
      sw.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC)
      sw.shadow_type = Gtk::SHADOW_IN
      table.attach(sw,
		   # X direction            # Y direction
		   0, 1,                    2, 3,
		   Gtk::EXPAND | Gtk::FILL, Gtk::EXPAND | Gtk::FILL,
		   0,                       0)

      set_default_size(200, 200)

      contents = Gtk::TextView.new
      sw.add(contents)

      ## Create statusbar
      statusbar = Gtk::Statusbar.new
      table.attach(statusbar,
		   # X direction            # Y direction
		   0, 1,                    3, 4,
		   Gtk::EXPAND | Gtk::FILL, 0,
		   0,                       0)

      # Show text widget info in the statusbar
      buffer = contents.buffer
      buffer.signal_connect('changed') do |buffer|
	update_statusbar(buffer, statusbar)
      end
      # cursor moved
      buffer.signal_connect('mark_set') do |buffer, iter, mark|
	update_statusbar(buffer, statusbar)
      end
    end

    def toolbar_cb(*args)
      dialog = Gtk::MessageDialog.new(self,
				      Gtk::Dialog::DESTROY_WITH_PARENT,
				      Gtk::MessageDialog::INFO,
				      Gtk::MessageDialog::BUTTONS_CLOSE,
				      "You selected a toolbar button")

      # Close dialog on user response
      dialog.signal_connect('response') do |widget, data|
	widget.destroy
      end

      dialog.show
    end

    def about_cb(*args)
      unless Gtk.check_version?(2, 6, 0)
        puts "This sample requires GTK+ 2.6.0 or later"
        return
      end

      authors = ["Peter Mattis", "Spencer Kimball", "Josh MacDonald", "and many more..."]
      documentors = ["Owen Taylor", "Tony Gale", "Mattias Clasen <mclasen@redhat.com>",  
	"and many more..."]
      license = %Q[
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with the Gnome Library; see the file COPYING.LIB.If not,
write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.
]

      transparent = Gdk::Pixbuf.new(Demo.find_file("gtk-logo-rgb.gif")).add_alpha(true, 0xff, 0xff, 0xff)
      
      Gtk::AboutDialog.set_email_hook {|about, link|
	puts "send mail to #{link}"
      }
      Gtk::AboutDialog.set_url_hook {|about, link|
	puts "show url #{link}"
      }
      Gtk::AboutDialog.show(self,
			    :name => "GTK+ Code Demos",
			    :version => Gtk::VERSION.join("."),
			    :copyright => "(C) 1997-2004 The GTK+ Team",
			    :license => license,
			    :website => "http://www.gtk.org",
			    :comments => "Program to demonstrate GTK+ functions.",
			    :authors => authors,
			    :documenters => documentors,
			    :logo => transparent)
    end

    def update_statusbar(buffer, statusbar)
      statusbar.pop(0)

      iter = buffer.get_iter_at_mark(buffer.get_mark('insert'))

      statusbar.push(0,
		     "Cursor at row #{iter.line} column #{iter.line_offset} - #{buffer.char_count} chars in document")
    end

    def register_stock_icons
      # Register our stock items
      Gtk::Stock.add(:demo_gtk_logo, '_GTK!')

      # Add out custom icon factory to the list of defaults
      factory = Gtk::IconFactory.new
      factory.add_default

      filename = Demo.find_file('gtk-logo-rgb.gif')
      pixbuf = Gdk::Pixbuf.new(filename)

      transparent = pixbuf.add_alpha(true, 0xff, 0xff, 0xff)

      icon_set = Gtk::IconSet.new(transparent)
      factory.add('demo_gtk_logo', icon_set)
    end
  end
end
