#!/usr/bin/env ruby
=begin
  style_property.rb - Ruby/GTK sample script.

  Copyright (c) 2004-2015 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
=end

require "gtk3"

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
                                              0,     # min
                                              100,   # max
                                              5,     # default
                                              GLib::Param::READABLE |
                                              GLib::Param::WRITABLE)) do |pspec, str|
    p pspec, str
    str.to_i + 10  # return the converted value.
  end

  install_style_property(GLib::Param::Enum.new("bar", # name
                                               "Bar", # nick
                                               "BAR", # blurb
                                               GLib::Type["GdkCursorType"], # Enum type
                                               Gdk::CursorType::ARROW, # default
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

provider = Gtk::CssProvider.new
provider.load(:data => <<-CSS)
* {
  -MyButton-foo: 30;
  -MyButton-bar: boat;
}
CSS

display = Gdk::Display.default
screen = display.default_screen
Gtk::StyleContext.add_provider_for_screen(screen, provider, Gtk::StyleProvider::PRIORITY_USER)

win = Gtk::Window.new("Custom style properties")
b = MyButton.new("Hello")
b.signal_connect("clicked") { Gtk.main_quit }

p MyButton.style_properties

win.set_default_size(100, 100)
win.add(b)
win.show_all
win.signal_connect("destroy") { Gtk.main_quit }

# You need to call them after "Gtk::Widget#show"
# (Or in expose event).
p b.style_get_property("foo")
p b.style_get_property("bar")

Gtk.main
