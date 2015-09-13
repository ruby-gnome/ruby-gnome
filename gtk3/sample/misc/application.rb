#!/usr/bin/env ruby
=begin
  application.rb - Ruby/GTK version of the example-1.c at
  https://developer.gnome.org/gtk3/stable/gtk-getting-started.html
  Copyright (c) 2015  Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
=end

require "gtk3"
myapp = Gtk::Application.new("org.gtk.example", :handles_command_line)
myapp.signal_connect "command-line" do |app, command_line|
  app.activate
  0
end

myapp.signal_connect "activate" do |app|
  win = Gtk::ApplicationWindow.new(app)
  win.set_title("window")
  win.set_default_size 200, 200

  button_box = Gtk::ButtonBox.new(:horizontal)
  win.add(button_box)

  button = Gtk::Button.new(:label => "Hello World")
  button.signal_connect "clicked" do
    puts "Hello World"
    win.destroy
  end

  button_box.add(button)

  win.show_all
end

myapp.run
