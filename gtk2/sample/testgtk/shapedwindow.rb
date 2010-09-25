=begin header

  shapedwindow.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002-2005 Ruby-GNOME2 Project Team

  $Id: shapedwindow.rb,v 1.7 2005/07/21 17:47:19 mutoh Exp $

=end

require 'sample'

class ShapeSampleBasic < Gtk::Window
  include Sample
  extend SampleClass

  @@root_win = nil

  def initialize(xpm_file, x, y, px, py, type)
    super(type)
    @destroyed = false
    signal_connect("destroy"){destroy}

    realize

    @@root_win = Gdk::Window.default_root_window unless @@root_win

    window.set_events(window.events | Gdk::Event::BUTTON_MOTION_MASK |
	       Gdk::Event::POINTER_MOTION_HINT_MASK |
	       Gdk::Event::BUTTON_PRESS_MASK)

    signal_connect("button_press_event") do |w, event|
      if (event.event_type == Gdk::Event::BUTTON_PRESS)
	@x = event.x
	@y = event.y
	Gtk.grab_add(self)
	Gdk.pointer_grab(window, true,
			    Gdk::Event::BUTTON_RELEASE_MASK |
			    Gdk::Event::BUTTON_MOTION_MASK |
			    Gdk::Event::POINTER_MOTION_HINT_MASK,
			    nil, nil, 0)
      end
    end
    signal_connect("button_release_event") do
      Gtk.grab_remove(self)
      Gdk.pointer_ungrab(0)
    end
    signal_connect("motion_notify_event") do
      xp, yp, mask = @@root_win.pointer
      move(xp  - @x, yp  - @y)
    end

    move(x, y)

    style = Gtk::Widget.default_style

    fixed = Gtk::Fixed.new
    fixed.set_size_request(100, 100)
    add(fixed)

    gdk_pixmap, gdk_pixmap_mask =
      Gdk::Pixmap::create_from_xpm(window,
				   style.bg(Gtk::STATE_NORMAL),
				   xpm_file)
    pixmap = Gtk::Image.new(gdk_pixmap, gdk_pixmap_mask)

    fixed.put(pixmap, px, py)

    shape_combine_mask(gdk_pixmap_mask, px, py)
  end
end

class ShapeSampleModeller < ShapeSampleBasic
  def initialize
    super("Modeller.xpm", 440, 140, 0, 0, Gtk::Window::POPUP)
  end
end

class ShapeSampleSheets < ShapeSampleBasic
  def initialize
    super("FilesQueue.xpm", 580, 170, 0, 0, Gtk::Window::POPUP)
  end
end

class ShapeSampleRings < ShapeSampleBasic
  def initialize
    super("3DRings.xpm", 460, 270, 25, 25, Gtk::Window::TOPLEVEL)
  end
end


ShapesSample = Class.new
class << ShapesSample
  def invoke
    ShapeSampleModeller.invoke
    ShapeSampleSheets.invoke
    ShapeSampleRings.invoke
  end
end
