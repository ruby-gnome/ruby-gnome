#!/usr/bin/ruby
=begin
  hello-applet.rb 

  Copyright (c) 2004 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: hello-applet.rb,v 1.1 2004/06/06 17:23:04 mutoh Exp $
=end


require 'panelapplet2'

PanelApplet.main("OAFIID:GNOME_HelloApplet_Factory",
                 "The Hello World Applet", "0.1") do |applet, iid|
  if iid == "OAFIID:GNOME_HelloApplet"
    label = Gtk::Label.new("Hello World")
    applet.add(label)
    applet.show_all
    true
  else
    false
  end
end
