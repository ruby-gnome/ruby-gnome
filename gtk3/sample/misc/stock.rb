=begin
  stock.rb - Gtk::Stock sample script.

  Copyright (C) 2001-2006 Masao Mutoh
  Copyright (c) 2001-2015 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
=end

require "gtk3"

window = Gtk::Window.new("Gtk::Stock sample")
window.signal_connect("destroy") { Gtk.main_quit }

stocks = Gtk::Stock.constants.sort

image = Gtk::Image.new(:stock => Gtk::Stock::OK, :size => :dialog)
label = Gtk::Label.new("Gtk::Stock::OK")
button = Gtk::Button.new(:label => "Click!")
cnt = 0
button.signal_connect("clicked") do
  stock_name = "Gtk::Stock::#{stocks[cnt]}"
  label.set_text(stock_name)
  image.set_stock(eval(stock_name))
  if cnt < stocks.size - 1
    cnt += 1
  else
    cnt = 0
  end
end

box = Gtk::Box.new(:vertical, 0)
box.add(image)
box.add(label)
box.add(button)
window.add(box)
window.set_default_size(200, 200)
window.show_all

Gtk.main
