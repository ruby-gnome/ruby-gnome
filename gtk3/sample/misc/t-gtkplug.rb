#!/usr/bin/env ruby
=begin
  Sample script using Gtk::Socket and Gtk::Plug.

  $ ruby t-gtksocket.rb

  Written by Alex Boussinet <mailto:dbug@wanadoo.fr> for testing purpose only.

  Copyright (c) 2003-2015 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
=end

require "gtk3"

class MyGtkPlug
  def initialize(xid, plug)
    plug = "Button" if plug.nil?
    if xid.nil?
      @window = Gtk::Window.new("Gtk::Plug Test")
      @window.set_default_size(250, 50)
    else
      @window = Gtk::Plug.new(xid.to_i)
    end
    @window.window_position = Gtk::WindowPosition::CENTER
    @window.signal_connect("delete_event") { Gtk.main_quit }
    @vbox = Gtk::Box.new(:vertical, 5)
    @window.add(@vbox)
    @button1 = Gtk::Button.new(:label => plug)
    @button1.signal_connect("clicked") { $stderr.puts plug }
    @button2 = Gtk::Button.new(:label => "Exit")
    @button2.signal_connect("clicked") { Gtk.main_quit }
    # Exit button to test an unexpected end of child process by Gtk::Socket
    @vbox.pack_start(@button1, :expand => true, :fill => true)
    @vbox.pack_start(@button2, :expand => true, :fill => true)
    @window.show_all
  end
end

xid = nil
ARGV.each_index do |i|
  arg = ARGV.at(i)
  if arg == "-x"
    if arg.length > 2
      xid = arg[2..-1]
    else
      xid = ARGV.at(i + 1)
      ARGV.delete_at(i + 1)
    end
    xid = nil if xid.to_i <= 0
    ARGV.delete_at(i)
  end
end

MyGtkPlug.new(xid, ARGV.shift)
Gtk.main
