# Copyright (c) 2008-2015 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
= test demo 

Demonstrates the demo interface.
=end
module TestModDemo

  def self.run_demo(window)
    puts "ok"
    window = Gtk::Window.new(:toplevel)
    window.add(Gtk::Label.new("This is a test"))

    if !window.visible?
      window.show_all
    else
      window.destroy
    end
  end
end
