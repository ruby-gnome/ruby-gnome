#!/usr/bin/env ruby
=begin
  bindings.rb - Ruby/GTK sample script.

  Copyright (c) 2002-2006 Ruby-GNOME2 Project Team 
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: bindings.rb,v 1.7 2006/06/17 13:18:12 mutoh Exp $
=end

=begin
Usage:
  bindings.rb <filename>

  Following key bindings are effective in the TextView area.
  
    <space>      : scroll down by one page
    <backspace>  : scroll up by one page
    j            : move cursor donw by one line
    k            : move cursor up by one line

  clicking buttons cause following effect

    "space"      : same as pressing <space> in text view area.
    "back_space" : same as pressing <backspace> in text view area.
    "cancel j/k" : disable 'j' and 'k' binding
=end
require 'gtk2'

class Pager < Gtk::TextView
  type_register
  
  # widget's key binding can be defined like this
  binding_set.add_signal(Gdk::Keyval::GDK_space, 0,
                         "move_cursor", 
                         Gtk::MOVEMENT_PAGES, 1, false)
  binding_set.add_signal(Gdk::Keyval::GDK_BackSpace, 0,
                         "move_cursor", 
                         Gtk::MOVEMENT_PAGES, -1, false)

  def initialize(path)
    @path = path
    super()
    @buffer = self.buffer
    load
    set_editable(false)
    set_size_request(400, 400)
  end

  def load
    open(@path).read.each_line do |line|
      @buffer.insert_at_cursor(line)
    end
    @buffer.place_cursor(@buffer.start_iter)
  end
end

path = ARGV[0] || __FILE__ 

window = Gtk::Window.new
window.name = "pager_window"
sw = Gtk::ScrolledWindow.new
vbox = Gtk::VBox.new
hbox = Gtk::HBox.new
pager = Pager.new(path)

hbox.add(button1 = Gtk::Button.new("space"))
hbox.add(button2 = Gtk::Button.new("back_space"))
hbox.add(button3 = Gtk::Button.new("cancel j/k"))

button1.signal_connect("clicked") do
  Pager.binding_set.activate(Gdk::Keyval::GDK_space, 0, pager)
end
button2.signal_connect("clicked") do
  pager.bindings_activate(Gdk::Keyval::GDK_BackSpace, 0)
end

# Key bindings can be attached to any widget by 
# Gtk::BindingSet#add_path
# see RC Files section of GTK+ documentation for more detail.
bset = Gtk::BindingSet.new("j_and_k")
bset.add_signal(Gdk::Keyval::GDK_j, 0,
                "move_cursor",
                Gtk::MOVEMENT_DISPLAY_LINES, 1, false)
bset.add_signal(Gdk::Keyval::GDK_k, 0,
                "move_cursor",
                Gtk::MOVEMENT_DISPLAY_LINES, -1, false)
bset.add_path(Gtk::PathType::WIDGET, "pager_window.*.Pager", 
                 Gtk::PathPriorityType::APPLICATION)

button3.signal_connect("clicked") do
  bset.entry_clear(Gdk::Keyval::GDK_j, 0)
  bset.entry_clear(Gdk::Keyval::GDK_k, 0)
end

sw.add(pager)
vbox.add(hbox).add(sw)
window.add(vbox)
window.show_all

pager.grab_focus

window.signal_connect("destroy") { Gtk.main_quit }

Gtk.main


