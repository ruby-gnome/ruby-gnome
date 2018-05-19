# Copyright (c) 2008-2015 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
= test demo 

Demonstrates the demo interface.
=end

class TestModDemo
  def initialize(window)
    puts "ok"
    @window = Gtk::Window.new(:toplevel)
    @window.add(Gtk::Label.new("This is a test"))
  end

  def run
    if !@window.visible?
      @window.show_all
    else
      @window.destroy
    end
    @window
  end
end
