#!/usr/bin/env ruby
=begin
  label.rb - Ruby/GTK3 sample script.

  Copyright (c) 2002-2015 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
=end

require "gtk3"

win = Gtk::Window.new("Gtk::Label sample")
win.signal_connect("destroy"){Gtk.main_quit}
label1 = Gtk::Label.new("_Not Use Mnemonic", :use_underline => false)
label1.selectable = true
label2 = Gtk::Label.new("_Use Mnemonic", :use_underline => true)
label3 = Gtk::Label.new.set_markup(%Q[<markup><span size="xx-large" weight="bold" foreground="blue"><u>Ruby-GNOME2</u></span> is <span foreground="red" size="x-large"><i>cool</i></span>!</markup>])
label4 = Gtk::Label.new.set_markup(%Q[<span foreground="#999900">_Not Use Mnemonic</span>], :use_underline => false)
label5 = Gtk::Label.new.set_markup(%Q[<span foreground="#009999">_Use Mnemonic</span>], :use_underline => true)

box = Gtk::Box.new(:vertical)
box.add(label1)
box.add(label2)
box.add(label3)
box.add(label4)
box.add(label5)
win.add(box)
win.show_all

# This can be called after realized only.
label1.select_region(3, 8)

Gtk.main
