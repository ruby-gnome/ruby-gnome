=begin header

  layout.rb - a part of testgtk.c rewritten in ruby-gtk

  Rewritten by Minoru Inachi <inachi@earth.interq.or.jp>

Original Copyright:

  GTK - The GIMP Toolkit
  Copyright (C) 1995-1997 Peter Mattis, Spencer Kimball and Josh MacDonald
 
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.
 
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.
 
  You should have received a copy of the GNU Library General Public
  License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place - Suite 330,
  Boston, MA 02111-1307, USA.

=end

require 'sample'

class LayoutSample < SampleWindow

  def initialize
    super("Gtk::Layout")

    set_usize(200, 200)

    scrolledwindow = Gtk::ScrolledWindow.new(nil, nil)

    add(scrolledwindow)
      
    @layout = Gtk::Layout.new(nil, nil)
    scrolledwindow.add(@layout)

    # We set step sizes here since GtkLayout does not set
    # them itself.
    @layout.hadjustment.step_increment = 10.0
    @layout.vadjustment.step_increment = 10.0

    @layout.set_events(Gdk::EXPOSURE_MASK)
    @layout.signal_connect("expose_event") do
      | w, event |
      layout_expose_handler(event)
    end

    @layout.set_size(1600, 128000)
      
    for i in 0..15 do
      for j in 0..15 do
	buf = sprintf("Button %d, %d", i, j)
	if ((i + j) % 2) != 0 then
	  button = Gtk::Button.new(buf)
	else
	  button = Gtk::Label.new(buf)
        end

	@layout.put(button, j*100, i*100)
      end
    end

    for i in 16..1279 do
      buf = sprintf("Button %d, %d", i, 0)
      if (i % 2) != 0 then
	button = Gtk::Button.new(buf)
      else
	button = Gtk::Label.new(buf)
      end
      @layout.put(button, 0, i*100)
    end
  end

  private
  def layout_expose_handler(event)
    imin = (@layout.xoffset + event.area.x) / 10
    imax = (@layout.xoffset + event.area.x + event.area.width + 9) / 10

    jmin = (@layout.yoffset + event.area.y) / 10
    jmax = (@layout.yoffset + event.area.y + event.area.height + 9) / 10

    @layout.window.clear_area(event.area.x, event.area.y,
			      event.area.width, event.area.height)

    for i in imin..imax-1 do
      for j in jmin..jmax-1 do
        if ((i+j) % 2) != 0 then
	  @layout.bin_window.draw_rectangle(
			@layout.style.black_gc,
			true,
			10*i - @layout.xoffset, 10*j - @layout.yoffset, 
			1+i%10, 1+j%10)
	end
      end
    end
    true
  end

end
