=begin header

  rulers.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team

  $Id: rulers.rb,v 1.8 2003/10/14 13:34:08 mutoh Exp $

=end

require 'sample'

class RulerSample < SampleWindow
  def initialize
    super("rulers")
    set_default_size(300, 300)
    set_events(Gdk::Event::POINTER_MOTION_MASK | Gdk::Event::POINTER_MOTION_HINT_MASK)

    table = Gtk::Table::new(2, 2, false)
    add(table)
    table.show

    hruler = Gtk::HRuler::new()
    hruler.set_metric(Gtk::CENTIMETERS)
    hruler.set_range(100, 0, 0, 20)
    vruler = Gtk::VRuler::new()
    vruler.set_range(5, 15, 0, 20)
    signal_connect("motion_notify_event") do |w, e|
      vruler.signal_emit("motion_notify_event", e)
    end
    table.attach(hruler, 1, 2, 0, 1,
		 Gtk::EXPAND | Gtk::FILL, Gtk::FILL, 0, 0)
    hruler.show

    signal_connect("motion_notify_event") do |w, e|
       hruler.signal_emit("motion_notify_event", e)
    end
    table.attach(vruler, 0, 1, 1, 2,
		 Gtk::FILL, Gtk::EXPAND | Gtk::FILL, 0, 0)
    vruler.show
  end
end
