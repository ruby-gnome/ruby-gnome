#  -*- indent-tabs-mode: nil -*-

# Original copyright
#
# canvas-curve.c: bezier curve demo.
#
# Copyright (C) 2002 Mark McLoughlin
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.
#
# Authors:
#     Mark McLoughlin <mark@skynet.ie>

class CanvasSampleBezierCurve < Gtk::VBox

  STATE_INIT = 0
  STATE_FIRST_PRESS = 1
  STATE_FIRST_RELEASE = 2
  STATE_SECOND_PRESS = 3

  def draw_curve(item, x, y)
    root = item.parent
    
    case @current_state
    when STATE_INIT
      @current_points = [] if @current_points.nil?

      @current_points[0] = x
      @current_points[1] = y
    when STATE_FIRST_PRESS
      @current_points[2] = x
      @current_points[3] = y

      path_def = Gnome::CanvasPathDef.new()

      path_def.moveto(@current_points[0], @current_points[1])
      path_def.lineto(@current_points[2], @current_points[3])

      unless @current_item.nil?
        @current_item.set({:bpath => path_def})
      else
        @current_item = Gnome::CanvasBpath.new(root,
                                               {:bpath => path_def,
                                                 :outline_color => "blue",
                                                 :width_pixels => 5,
                                                 :cap_style => Gdk::GC::CAP_ROUND})
        @current_item.signal_connect("event") do |item, event|
          item_event(item, event)
        end
      end
    when STATE_FIRST_RELEASE
      @current_points[4] = x
      @current_points[5] = y

      path_def = Gnome::CanvasPathDef.new()

      path_def.moveto(@current_points[0], @current_points[1])
      path_def.curveto(@current_points[4], @current_points[5],
                       @current_points[4], @current_points[5],
                       @current_points[2], @current_points[3])

      @current_item.set({:bpath => path_def})

    when STATE_SECOND_PRESS
      @current_points[6] = x;
      @current_points[7] = y;

      path_def = Gnome::CanvasPathDef.new ();

      path_def.moveto(@current_points[0], @current_points[1])

      path_def.curveto(@current_points[4], @current_points[5],
                       @current_points[6], @current_points[7],
                       @current_points[2], @current_points[3])

      @current_item.set({:bpath => path_def})
      @current_item = nil
      
    else
      raise "not reached here"
    end
  end

  def item_event(item, event)
    if (event.event_type == Gdk::Event::BUTTON_PRESS &&
        event.button == 1 &&
        (event.state & Gdk::Window::SHIFT_MASK == Gdk::Window::SHIFT_MASK))
      if (item == @current_item)
        @current_item = nil
        @current_state = STATE_INIT
      end

      item.destroy()
      return true
    end
    false
  end


  def canvas_event(item, event)
    case event.event_type
    when Gdk::Event::BUTTON_PRESS
      return false if event.button != 1

      case @current_state
      when STATE_INIT
        draw_curve(item, event.x, event.y)
        @current_state = STATE_FIRST_PRESS
      when STATE_FIRST_RELEASE
        draw_curve (item, event.x, event.y)
        @current_state = STATE_SECOND_PRESS
      when STATE_SECOND_PRESS
        draw_curve (item, event.x, event.y)
        @current_state = STATE_INIT
      else
        raise format("shouldn't have reached here %d", @current_state)
      end
    when Gdk::Event::BUTTON_RELEASE
      return false if event.button != 1

      case @current_state
      when STATE_FIRST_PRESS
        draw_curve (item, event.x, event.y)
        @current_state = STATE_FIRST_RELEASE
      end
    when Gdk::Event::MOTION_NOTIFY
      case @current_state
      when STATE_FIRST_PRESS
        draw_curve (item, event.x, event.y)
      end
    end
    false
  end

  def create_canvas(aa)
    Gtk::Widget.push_colormap(Gdk::RGB.cmap())

    canvas = Gnome::Canvas.new(aa)

    canvas.set_size_request(600, 250) #?
    canvas.set_scroll_region(0, 0, 600, 250)
    canvas.show()

    root = canvas.root

    item = Gnome::CanvasRect.new(root,
                                 {:outline_color => "black",
                                   :fill_color => "white",
                                   :x1 => 0.0,
                                   :y1 => 0.0,
                                   :x2 => 600.0,
                                   :y2 => 250.0})

    Gnome::CanvasText.new(root,
                          {:text => aa ? "AntiAlias" : "Non-AntiAlias",
                            :x => 270.0,
                            :y => 5.0,
                            :font => "Sans 12",
                            :anchor => Gtk::ANCHOR_N,
                            :fill_color => "black"})

    Gtk::Widget.pop_colormap()

    item.signal_connect("event") do |item, event|
      canvas_event(item, event)
    end

    frame = Gtk::Frame.new()
    frame.shadow_type = Gtk::SHADOW_IN

    frame.add(canvas)
    frame
  end

  def initialize
    super(false, 4)
    self.border_width = 4
    self.show()

    @current_state = STATE_INIT

    label = Gtk::Label.new(<<EOS)
Drag a line with button 1. Then mark 2 control points with
button 1. Shift+click with button 1 to destroy the curve.
EOS
    self.pack_start(label, false, false, 0)
    label.show()

    canvas = create_canvas(false)
    self.pack_start(canvas, true, true, 0)
    canvas.show()

    aa_canvas = create_canvas(true)
    self.pack_start(aa_canvas, true, true, 0)
    aa_canvas.show()
  end
end
