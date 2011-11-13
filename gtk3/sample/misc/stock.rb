=begin
  stock.rb - Gtk::Stock sample script.

  Copyright (C) 2001-2006 Masao Mutoh
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: stock.rb,v 1.7 2006/06/17 13:18:12 mutoh Exp $
=end

require 'gtk2'

window = Gtk::Window.new("Gtk::Stock sample")
window.signal_connect("destroy"){Gtk.main_quit}

stocks = Gtk::Stock.constants.sort

image = Gtk::Image.new
label = Gtk::Label.new
button = Gtk::Button.new("Click!")
cnt = 0
button.signal_connect("clicked") do
  stock_name = "Gtk::Stock::#{stocks[cnt]}"
  label.set_text(stock_name)
  image.set(eval(stock_name), Gtk::IconSize::DIALOG)
  if cnt < stocks.size - 1
    cnt += 1
  else
    cnt = 0
  end
end

box = Gtk::VBox.new.add(image).add(label).add(button)
window.add(box).set_default_size(200,200).show_all

Gtk.main

