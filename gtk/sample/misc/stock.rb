=begin
  stock.rb - Gtk::Stock sample script.

  Copyright (C) 2001,2002 Masao Mutoh<mutoh@highway.ne.jp>
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: stock.rb,v 1.2 2002/11/23 04:31:22 mutoh Exp $
=end

require 'gtk2'

Gtk.init

window = Gtk::Window.new

stocks = Gtk::Stock.constants.sort

image = Gtk::Image.new
label = Gtk::Label.new
button = Gtk::Button.new("Click!")
cnt = 0
button.signal_connect("clicked") do
  stock_name = "Gtk::Stock::#{stocks[cnt]}"
  label.text = stock_name 
  image.set(eval(stock_name))
  if cnt < stocks.size - 1
    cnt += 1
  else
    cnt = 0
  end
end

box = Gtk::VBox.new.add(image).add(label).add(button)
window.add(box).set_default_size(200,200).show_all

Gtk.main

