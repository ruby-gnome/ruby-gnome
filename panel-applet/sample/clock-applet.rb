#!/usr/bin/env ruby
=begin
  clock-applet.rb
                                                                                
  Copyright (c) 2004 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
                                                                                
  $Id: clock-applet.rb,v 1.2 2004/06/06 17:23:04 mutoh Exp $
=end


require 'panelapplet2'

OAFIID="OAFIID:GNOME_SampleClockApplet_Factory"

init = proc do |applet, iid|
  label = Gtk::Label.new

  applet.add(label)
  applet.show_all
  
  Thread.start do
    loop do
      time = Time.now
      str = time.strftime("%m/%d (%a) %H:%M:%S")
      label.set_text("#{str}")
      sleep(1)
    end
  end
  true
end

oafiid = OAFIID
run_in_window = (ARGV.length == 1 && ARGV.first == "run-in-window")
oafiid += "_debug" if run_in_window

PanelApplet.main(oafiid, "Sample Clock Applet (Ruby-GNOME2)", "0", &init)

if run_in_window
  main_window = Gtk::Window.new
  main_window.set_title "Sample Clock Applet"
  main_window.signal_connect("destroy") { Gtk::main_quit }
  app = PanelApplet.new
  init.call(app, oafiid)
  app.reparent(main_window)
  main_window.show_all
  Gtk::main
end
