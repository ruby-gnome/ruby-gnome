#!/usr/bin/env ruby
=begin
  threads.rb - Ruby/GTK sample script.

  Copyright (c) 2003-2015 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
=end

require "gtk3"
require "thread"

label = Gtk::Label.new

Thread.new do
  (0...1000).each do |cnt|
    p "A:" + cnt.to_s
    label.label = "A:" + cnt.to_s
    sleep(2)
  end
end

start_button = Gtk::Button.new(:label => "start")
stop_button = Gtk::Button.new(:label => "stop")

start_button.signal_connect("clicked") do
  start_button.sensitive = false
  @th = Thread.new do
    (0...10).each do |cnt|
      p "B:" + cnt.to_s
      label.label = "B:" + cnt.to_s
      sleep(2)
    end
    @th = nil
    start_button.sensitive = true
    start_button.grab_focus
    stop_button.sensitive = false
  end
  stop_button.sensitive = true
  stop_button.grab_focus
end

stop_button.signal_connect("clicked") do
  if @th
    @th.kill
    puts "killed"
    @th = nil
    start_button.sensitive = true
    start_button.grab_focus
    stop_button.sensitive = false
  end
end

stop_button.sensitive = false

box = Gtk::Box.new(:vertical, 0)
box.set_size_request(100, 100)

box << label << start_button << stop_button

win = Gtk::Window.new << box

win.show_all.signal_connect("delete_event") do
  p "Exiting..."
  Gtk.main_quit
  Thread.list.each { |t| t.kill if t != Thread.main }
end

Gtk.main
