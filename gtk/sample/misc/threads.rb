#!/usr/local/bin/ruby
=begin
  threads.rb - Ruby/GTK sample script.

  Copyright (c) 2003 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: threads.rb,v 1.1 2003/03/08 17:56:01 mutoh Exp $
=end

require 'gtk2'
require 'thread'

Gtk.init

label = Gtk::Label.new

Thread.new {
  (0...1000).each { |cnt|
    label.label = "A:" + cnt.to_s
    sleep(2)
    p cnt
    cnt += 1
  }
}

start_button = Gtk::Button.new("start")
start_button.signal_connect("clicked") do
  @th = Thread.new {
    (0...10).each { |cnt|
      label.label = "B:" + cnt.to_s
      sleep(2)
      p cnt
      cnt += 1
    }
    @th = nil
  }
end
  
stop_button = Gtk::Button.new("stop")
stop_button.signal_connect("clicked") do
  if @th
    @th.kill
    puts "killed"
    @th = nil
  end
end

box = Gtk::VBox.new.
  set_size_request(100,100).
  add(label).
  add(start_button).
  add(stop_button)

Gtk::Window.new.add(box).show_all

Gtk.main



