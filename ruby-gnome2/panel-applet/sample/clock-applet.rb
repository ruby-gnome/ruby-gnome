#!/usr/bin/env ruby

if $DEBUG
  require 'gtk'
else
  require 'panel-applet'
end

applet =
  if $DEBUG
    Gtk::Window.new
  else
    Gnome::AppletWidget.new("ruby-clock-applet")
  end
def applet.delete_event(e)
  if $DEBUG
    Gtk.main_quit
  else
    Gnome::AppletWidget.main_quit
  end
end
button = Gtk::Button.new
button.set_relief(Gtk::RELIEF_NONE)
button.show

label = Gtk::Label.new("")
button.add(label)
label.show

applet.add(button)
applet.show

Thread.start do
  loop do
    time = Time.now
    str = time.strftime("%m/%d (%a)\n%H:%M:%S")
    label.set_text("#{str}")
    sleep(1)
  end
end

if $DEBUG
  Gtk.main
else
  Gnome::AppletWidget.main # use instead of Gtk.main
end
