require 'sample'

#
# Shaped Windows
#

$root_win = nil

class ShapeSampleBasic < Gtk::Window
  include Sample
  extend SampleClass

  def initialize(xpm_file, x, y, px, py, type)
    super(type)
    border_width(0)
    @destroyed = false
    signal_connect("destroy") do destroy end

    set_events(get_events | Gdk::BUTTON_MOTION_MASK |
	       Gdk::POINTER_MOTION_HINT_MASK |
	       Gdk::BUTTON_PRESS_MASK)
    signal_connect("button_press_event") do |w, event|
      if (event.event_type == Gdk::BUTTON_PRESS)
	@x = event.x
	@y = event.y
	grab_add
	window.pointer_grab(true,
			    Gdk::BUTTON_RELEASE_MASK |
			    Gdk::BUTTON_MOTION_MASK |
			    Gdk::POINTER_MOTION_HINT_MASK,
			    nil, nil, 0)
      end
    end
    signal_connect("button_release_event") do
      grab_remove
      window.pointer_ungrab(0)
    end
    signal_connect("motion_notify_event") do
      xp, yp, mask = $root_win.get_pointer
      set_uposition(xp  - @x, yp  - @y)
    end

    set_uposition(x, y)

    style = Gtk::Widget::get_default_style()

    fixed = Gtk::Fixed::new()
    fixed.set_usize(100, 100)
    add(fixed)
    fixed.show

    realize
    gdk_pixmap, gdk_pixmap_mask =
      Gdk::Pixmap::create_from_xpm(window,
				   style.bg(Gtk::STATE_NORMAL),
				   xpm_file)
    pixmap = Gtk::Pixmap::new(gdk_pixmap, gdk_pixmap_mask)
    fixed.put(pixmap, px, py)
    pixmap.show

    shape_combine_mask(gdk_pixmap_mask, px, py)
  end
end

class ShapeSampleModeller < ShapeSampleBasic
  def initialize
    super("Modeller.xpm", 440, 140, 0, 0, Gtk::WINDOW_POPUP)
  end
end

class ShapeSampleSheets < ShapeSampleBasic
  def initialize
    super("FilesQueue.xpm", 580, 170, 0, 0, Gtk::WINDOW_POPUP)
  end
end

class ShapeSampleRings < ShapeSampleBasic
  def initialize
    super("3DRings.xpm", 460, 270, 25, 25, Gtk::WINDOW_TOPLEVEL)
  end
end


ShapesSample = Class.new
class << ShapesSample
  def invoke
    $root_win = Gdk::Window::foreign_new(Gdk::Window::root_window()) if ! $root_win
    ShapeSampleModeller.invoke
    ShapeSampleSheets.invoke
    ShapeSampleRings.invoke
  end
end
