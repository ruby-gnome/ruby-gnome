=begin
  drawing.rb - Gtk::Drawing sample script.

  Copyright (c) 2002,2003 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: drawing.rb,v 1.5 2003/04/01 16:21:15 mutoh Exp $
=end

require 'gtk2'

class Canvas < Gtk::DrawingArea
  def initialize
    super
    signal_connect("expose_event") { |w,e| expose_event(w,e) }
    signal_connect("configure_event") { |w, e| configure_event(w,e) }
    @buffer = nil
    @bgc = nil
  end

  def expose_event(w,e)
    if ! @buffer.nil?
      rec = e.area
      w.window.draw_drawable(@bgc, @buffer, rec.x, rec.y,
			   rec.x, rec.y, rec.width, rec.height)
    end
    false
  end

  def clear(b = @buffer)
    return if b.nil?

    g = b.size
    @bgc = self.style.bg_gc(self.state) if @bgc.nil?
    if (g[0] > 0 && g[1] > 0)
      b.draw_rectangle(@bgc, true, 0,0, g[0], g[1])
    end
  end

  def configure_event(w,e)
    g = w.window.geometry
    if (g[2] > 0 && g[3] > 0)
      b = Gdk::Pixmap::new(w.window, g[2], g[3], -1)
      clear(b)
      if not @buffer.nil?
	g = @buffer.size
	b.draw_drawable(@bgc, @buffer, 0, 0, 0, 0, g[0], g[1])
      end
      @buffer = b
    end
    true
  end
end

class A < Canvas
  def initialize
    super
    signal_connect("button_press_event") { |w,e| pressed(w,e) }
    set_events(Gdk::Event::BUTTON_PRESS_MASK)
  end

  def pressed(widget, ev)
    if not @last.nil?
      @buffer.draw_line(widget.style.fg_gc(widget.state),
			@last.x, @last.y, ev.x, ev.y)

      x1,x2 = if (@last.x < ev.x)
	      then [@last.x, ev.x]
	      else [ev.x,    @last.x]
	      end
      y1,y2 = if (@last.y < ev.y)
	    then [@last.y, ev.y]
	    else [ev.y,    @last.y]
	    end
      widget.queue_draw_area(x1, y1, x2 - x1 + 1, y2 - y1 + 1)
    end
    @last = nil
    @last = ev
    true
  end
end

Gtk.init

window = Gtk::Window.new
window.signal_connect("destroy") { Gtk.main_quit }
window.realize
window.set_title("drawing test")

canvas = A.new
window.add(canvas)

window.show_all
Gtk::main
