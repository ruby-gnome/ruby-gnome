=begin
  stock.rb - Gtk::Stock sample script.

  Copyright (C) 2001-2006 Masao Mutoh
  Copyright (c) 2001-2020 Ruby-GNOME Project Team
  This program is licenced under the same licence as Ruby-GNOME.
=end

require "gtk3"

window = Gtk::Window.new("Gtk::Stock sample")
window.signal_connect("destroy") { Gtk.main_quit }

stocks = Gtk::Stock.constants.sort.cycle

image = Gtk::Image.new(:stock => Gtk::Stock::OK, :size => :dialog)
label = Gtk::Label.new("Gtk::Stock::OK")
button = Gtk::Button.new(:label => "Click!")

button.signal_connect("clicked") do
  stock = stocks.next
  stock_name = "Gtk::Stock::#{stock}"
  label.set_text(stock_name)
  image.set_stock(eval(stock_name))
end

box = Gtk::Box.new(:vertical, 0)
box.add(image, :expand => true)
box.add(label, :expand => true)
box.add(button)
window.add(box)
window.set_default_size(200, 200)
window.show_all

Gtk.main
