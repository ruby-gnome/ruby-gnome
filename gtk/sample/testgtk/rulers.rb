require 'sample'

#
# GtkRulers
#

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
    signal_connect("motion_notify_event") do |w, e|
      hruler.motion_notify_event(e)
    end
    table.attach(hruler, 1, 2, 0, 1,
		 Gtk::EXPAND | Gtk::FILL, Gtk::FILL, 0, 0)
    hruler.show

    vruler = Gtk::VRuler::new()
    vruler.set_range(5, 15, 0, 20)
    signal_connect("motion_notify_event") do |w, e|
      vruler.motion_notify_event(e)
    end
    table.attach(vruler, 0, 1, 1, 2,
		 Gtk::FILL, Gtk::EXPAND | Gtk::FILL, 0, 0)
    vruler.show
  end
end
