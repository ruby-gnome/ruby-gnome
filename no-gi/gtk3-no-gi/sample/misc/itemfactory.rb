#!/usr/bin/env ruby
=begin
  itemfactory.rb - Ruby/GTK sample script.

  Copyright (c) 2002-2006 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  *NOTE* Gtk::ItemFactory has been deprecated. Use Gtk::UIManager instead.

  $Id: itemfactory.rb,v 1.7 2006/06/17 13:18:12 mutoh Exp $
=end

require 'gtk3'

puts "Deprecated. Use Gtk::UIManager instead."

window = Gtk::Window.new("Gtk::ItemFactory")
window.signal_connect("destroy") do
  Gtk.main_quit
end
window.signal_connect("delete_event") do
  Gtk.main_quit
end

accelgroup = Gtk::AccelGroup.new
window.add_accel_group(accelgroup)

ifp = Gtk::ItemFactory.new(Gtk::ItemFactory::TYPE_MENU_BAR, "<main>", accelgroup)

cal_stock = Proc.new {|d, item| p "StockItem, #{d}"}
cal_quit = Proc.new{p "Quit"; Gtk.main_quit}
cal_check = Proc.new {|d, item| p "CheckItem, #{d}"}
cal_toggle = Proc.new {|d, item| p "ToggleItem, #{d}"}
cal_radio = Proc.new {|d, item| p "RadioItem, #{d}"}
cal_misc = Proc.new {|d, item| p "Misc #{d}"}
ifp.create_items([
                   ["/_Stock"],
                   ["/_Stock/_New", "<StockItem>", "<control>N", Gtk::Stock::NEW, cal_stock, 1],
                   ["/_Stock/_Open", "<StockItem>", "<control>O", Gtk::Stock::OPEN, cal_stock, 2],
                   ["/_Stock/_Quit", "<StockItem>", "<control>Q", Gtk::Stock::QUIT, cal_quit],

                   ["/_Check"],
                   ["/_Check/Check_1", "<CheckItem>", "", "", cal_check, 1],
                   ["/_Check/Check_2", "<CheckItem>", "", "", cal_check, 2],

                   ["/_Toggle"],
                   ["/_Toggle/Toggle_1", "<ToggleItem>", "", "", cal_toggle, 1],
                   ["/_Toggle/Toggle_2", "<ToggleItem>", "", "", cal_toggle, 2],
                   ["/_Toggle/Toggle_3", "<ToggleItem>", "", "", cal_toggle, 3],

                   ["/_Radio", "<Branch>"],
                   ["/_Radio/Radio_1", "<RadioItem>", nil, nil, cal_radio, 1],
                   ["/_Radio/Radio_2", "/Radio/Radio1", nil, nil, cal_radio, 2],
                   ["/_Radio/Radio_3", "/Radio/Radio1", nil, nil, cal_radio, 3],

                   ["/_Misc", "<LastBranch>"],
                   ["/_Misc/Tearoff", "<Tearoff>"],
                   ["/_Misc/Title", "<Title>"],
                   ["/_Misc/Image", "<ImageItem>", "", Gdk::Pixbuf.new("gnome-logo-icon.png"), cal_misc, 1],
                   ["/_Misc/Separator", "<Separator>"],
                   ["/_Misc/Item", "<Item>", nil, nil, cal_misc, 2]
                 ])

ifp.get_widget("/Stock/Open").sensitive = false
ifp.delete_item("/Toggle/Toggle2")

vbox = Gtk::VBox.new
vbox.add(ifp.get_widget("<main>"))
vbox.add(Gtk::Label.new("Gtk::ItemFactory sample").set_size_request(400, 200))
window.add(vbox)
window.show_all

Gtk.main
