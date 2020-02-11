=begin
  eventbox.rb - Ruby/GTK sample script.

  Copyright (c) 2015-2020 Ruby-GNOME Project Team
  This program is licenced under the same licence as Ruby-GNOME.
=end

# https://developer.gnome.org/gtk3/unstable/GtkEventBox.html
# https://developer.gnome.org/gtk3/stable/GtkImage.html

# Gtk::EventBox are widgets container used to catch events for widgets that
# do not have their own window (Gdk::Window) like Gtk::Image for example.

require "gtk3"

window = Gtk::Window.new("Event Box example")

surface = Cairo::ImageSurface.new(Cairo::FORMAT_ARGB32, 265, 95)
cr = Cairo::Context.new(surface)
r1 = [10, 10, 75, 75]
r2 = [95, 10, 75, 75]
r3 = [180, 10, 75, 75]

cr.set_source_rgb(1, 0, 0)
cr.rectangle(*r1)
cr.fill
cr.set_source_rgb(0, 1, 0)
cr.rectangle(*r2)
cr.fill
cr.set_source_rgb(0, 0, 1)
cr.rectangle(*r3)
cr.fill

image = Gtk::Image.new(
  pixbuf: surface.to_pixbuf(src_x: 0, src_y: 0, width: 265, height: 95)
)

event_box = Gtk::EventBox.new

event_box.add(image)

event_box.signal_connect "button-press-event" do |_widget, event|
  if event.y >= 10 && event.y <= 85
    if event.x >= 10 && event.x <= 85
      puts "red   x = #{event.x} \t y = #{event.y}"
    elsif event.x >= 95 && event.x <= 180
      puts "green x = #{event.x} \t y = #{event.y}"
    elsif event.x >= 190 && event.x <= 255
      puts "blue  x = #{event.x} \t y = #{event.y}"
    end
  end
end

window.add(event_box)

window.show_all

window.signal_connect("delete-event") { Gtk.main_quit }

Gtk.main
