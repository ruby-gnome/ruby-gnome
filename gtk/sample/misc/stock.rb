=begin
  stock.rb - Gtk::Stock sample script.

  Copyright (C) 2001-2005 Masao Mutoh<mutoh@highway.ne.jp>
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: stock.rb,v 1.6 2005/07/14 17:01:49 mutoh Exp $
=end

require 'gtk2'

Gtk.init

window = Gtk::Window.new("Gtk::Stock sample")

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

