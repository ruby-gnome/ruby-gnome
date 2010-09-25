#!/usr/bin/env ruby
=begin
  composited-windows.rb - Sample script translated from http://library.gnome.org/devel/gdk/unstable/gdk-Windows.html#composited-window-example

  Copyright (c) 2008 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
=end

require 'gtk2'

if str = Gtk.check_version(2, 12, 0)
  puts "This sample requires GTK+ 2.12.0 or later."
  puts str
  exit
end

unless Gdk.cairo_available?
  puts "This sample requires Cairo support."
  exit
end


Gtk.init

# Make the widgets
button = Gtk::Button.new("A Button")
event = Gtk::EventBox.new
window = Gtk::Window.new(Gtk::Window::TOPLEVEL)

# Put a red background on the window
window.modify_bg(Gtk::STATE_NORMAL, Gdk::Color.parse("red"))

# Set the colourmap for the event box. Must be done before the event box is realised.
event.colormap = event.screen.rgba_colormap

# Set our event box to have a fully-transparent background
# drawn on it. Currently there is no way to simply tell GTK+
# that "transparency" is the background colour for a widget.
event.app_paintable = true

event.signal_connect('expose-event') do |widget, event|
  # This function simply draws a transparency onto a widget on the area
  # for which it receives expose events.  This is intended to give the
  # event box a "transparent" background.
  #
  # In order for this to work properly, the widget must have an RGBA
  # colourmap.  The widget should also be set as app-paintable since it
  # doesn't make sense for GTK+ to draw a background if we are drawing it
  # (and because GTK+ might actually replace our transparency with its
  # default background colour).
  #
  cr = widget.window.create_cairo_context
  cr.operator = Cairo::OPERATOR_CLEAR
  cr.gdk_region(event.region)
  cr.fill
  false
end

# Put them inside one another
window.border_width = 10
window.add(event)
event.add(button)

# Realize and show everything
window.show_all

# Set the event box GdkWindow to be composited.
# Obviously must be performed after event box is realised.
event.window.composited = true

# Set up the compositing handler.
# Note that we do _after_ so that the normal (red) background is drawn
# by gtk before our compositing occurs.
#
window.signal_connect_after('expose-event') do |widget, event|
  # This function performs the actual compositing of the event box onto
  # the already-existing background of the window at 50% normal opacity.
  #
  # In this case we do not want app-paintable to be set on the widget
  # since we want it to draw its own (red) background. Because of this,
  # however, we must ensure that we use #signal_connect_after so that
  # this handler is called after the red has been drawn. If it was
  # called before then GTK would just blindly paint over our work.
  #
  # Note: if the child window has children, then you need a cairo 1.16
  # feature to make this work correctly.
  #

  # get our child (in this case, the event box)
  child = widget.child

  # create a cairo context to draw to the window
  cr = widget.window.create_cairo_context

  # the source data is the (composited) event box
  # NOTICE: next line generates a GTK warning, it is maybe a GTK problem, it is tracked
  #         at http://bugzilla.gnome.org/show_bug.cgi?id=526965
  cr.set_source_pixmap(child.window, child.allocation.x, child.allocation.y)

  # draw no more than our expose event intersects our child
  region = Gdk::Region.new(Gdk::Rectangle.new(child.allocation.x, child.allocation.y, child.allocation.width, child.allocation.height))
  region.intersect(event.region)
  cr.gdk_region(region)
  cr.clip

  # composite, with a 50% opacity
  cr.operator = Cairo::OPERATOR_OVER
  cr.paint(0.5)

  false
end

Gtk.main
