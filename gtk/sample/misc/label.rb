#!/usr/local/bin/ruby
=begin
  label.rb - Ruby/GTK sample script.

  Copyright (c) 2002 Ruby-GNOME2 Project
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: label.rb,v 1.1 2002/12/01 15:51:01 mutoh Exp $
=end

require 'gtk2'

Gtk.init
win = Gtk::Window.new
label1 = Gtk::Label.new("_Not Use Mnemonic")
label1.selectable = true
label1.select_region(3, 8)
label2 = Gtk::Label.new("_Use Mnemonic", true)
label3 = Gtk::Label.new.set_markup(%Q[<markup><span size="xx-large" weight="bold" foreground="blue"><u>Ruby-GNOME2</u></span> is <span foreground="red" size="x-large"><i>cool</i></span>!</markup>], true)
label4 = Gtk::Label.new.set_markup(%Q[<span foreground="#999900">_Not Use Mnemonic</span>])
label5 = Gtk::Label.new.set_markup(%Q[<span foreground="#009999">_Use Mnemonic</span>], true)

box = Gtk::VBox.new
box.add(label1)
box.add(label2)
box.add(label3)
box.add(label4)
box.add(label5)

win.add(box).show_all

Gtk.main
