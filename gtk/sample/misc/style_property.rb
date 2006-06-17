#!/usr/bin/env ruby
=begin
  style_property.rb - Ruby/GTK2 sample script.

  Copyright (c) 2004,2006 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: style_property.rb,v 1.4 2006/06/17 13:18:12 mutoh Exp $
=end

require 'gtk2'

class MyButton < Gtk::Button
  type_register

  def initialize(label = nil)
    # When type_register() is used.
    # super is equivalent to GLib::Object#initialize.
    super("label" => label)
  end

  install_style_property(GLib::Param::Int.new("foo", # name
					      "Foo", # nick
					      "FOO", # blurb
					      0,     #min 
					      100,   #max
					      5,     #default
					      GLib::Param::READABLE |
					      GLib::Param::WRITABLE)) do |pspec, str|
    p pspec, str
    str.to_i + 10  #return the converted value.
  end

  install_style_property(GLib::Param::Enum.new("bar", # name
					       "Bar", # nick
					       "BAR", # blurb
					       GLib::Type["GdkCursorType"], #Enum type
					       Gdk::Cursor::ARROW, #default
					       GLib::Param::READABLE |
					       GLib::Param::WRITABLE)) do |pspec, str|
    p pspec, str
    if str.strip! == "boat"
      Gdk::Cursor::BOAT
    else
      pspec.default
    end
  end
end

Gtk::RC.parse("./style_property.rc")

win = Gtk::Window.new("Gtk::RC sample")
b = MyButton.new("Hello")
b.signal_connect("clicked"){ Gtk.main_quit }

p MyButton.style_properties

win.set_default_size(100, 100)
win.add(b).show_all
win.signal_connect("destroy"){ Gtk.main_quit }

# You need to call them after "Gtk::Widget#show"
# (Or in expose event).
p b.style_get_property("foo")
p cursor = b.style_get_property("bar")

Gtk.main
