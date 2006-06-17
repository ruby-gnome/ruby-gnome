#!/usr/bin/env ruby
=begin
  itemfactory2.rb - Ruby/GTK sample script.

  Copyright (c) 2002-2006 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  *NOTE* Gtk::ItemFactory has been deprecated. Use Gtk::UIManager instead.

  $Id: itemfactory2.rb,v 1.8 2006/06/17 13:18:12 mutoh Exp $
=end

require 'gtk2'

puts "Deprecated. Use Gtk::UIManager instead."

window = Gtk::Window.new("Gtk::ItemFactory2")
window.signal_connect("destroy") do
  Gtk.main_quit
end
window.signal_connect("delete_event") do
  Gtk.main_quit
end

accelgroup = Gtk::AccelGroup.new
window.add_accel_group(accelgroup)

ifp = Gtk::ItemFactory.new(Gtk::ItemFactory::TYPE_MENU_BAR, "<main>", accelgroup)

ifp.create_item("/_Stock")
ifp.create_item("/_Stock/_New", "<StockItem>", "<control>N", Gtk::Stock::NEW, 1) do |v, _|
  p "StockItem #{v}"
end
item = ifp.create_item("/_Stock/_Open", "<StockItem>", "<control>O", Gtk::Stock::OPEN, 2) do |v, _|
  p "StockItem #{v}"
end
ifp.create_item("/_Stock/_Quit", "<StockItem>", "<control>Q", Gtk::Stock::QUIT, 3) do |v, _|
  p "Quit"
  Gtk.main_quit
end

ifp.create_item("/_Item")
ifp.create_item("/_Item/Item_1", "<Item>"){p "Item1"}
ifp.create_item("/_Item/Item_2", "<Item>"){p "Item2"}

ifp.create_item("/_Check")
ifp.create_item("/_Check/Check_1", "<CheckItem>"){p "Check1"}
ifp.create_item("/_Check/Check_2", "<CheckItem>"){p "Check2"}

ifp.create_item("/_Toggle")
ifp.create_item("/_Toggle/Toggle_1", "<ToggleItem>", nil, nil, 1){|v, _| p "Toggle#{v}"}
ifp.create_item("/_Toggle/Toggle_2", "<ToggleItem>", nil, nil, 2){|v, _| p "Toggle#{v}"}

ifp.create_item("/_Radio")
ifp.create_item("/_Radio/Radio_1", "<RadioItem>", nil, nil, 1){|v, _| p "Radio#{v}"}
ifp.create_item("/_Radio/Radio_2", "/Radio/Radio1", nil, nil, 2){|v, _| p "Radio#{v}"}
ifp.create_item("/_Radio/Radio_3", "/Radio/Radio1", nil, nil, 3){|v, _| p "Radio#{v}"}

ifp.create_item("/_Misc", "<LastBranch>")
ifp.create_item("/_Misc/Tearoff", "<Tearoff>")
ifp.create_item("/_Misc/Title", "<Title>")
ifp.create_item("/_Misc/Seperator", "<Separator>")
ifp.create_item("/_Misc/Image", "<ImageItem>", "", Gdk::Pixbuf.new("gnome-logo-icon.png")) do 
  p "ImageItem"
end

ifp.get_widget("/Stock/Open").sensitive = false
vbox = Gtk::VBox.new
vbox.add(ifp.get_widget("<main>"))
vbox.add(Gtk::Label.new("Gtk::ItemFactory sample").set_size_request(400, 200))
window.add(vbox)
window.show_all

Gtk.main
