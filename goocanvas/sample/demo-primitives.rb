# -*- coding: utf-8 -*-
#  -*- indent-tabs-mode: nil -*-
=begin header

  Rewritten by  Emmanuel Pinault <seatmanu@yahoo.com>

Original Copyright:
 
  Author :  Richard Hestilow <hestgray@ionet.net>

  Copyright (C) 1998 Free Software Foundation

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2, or (at your option)
  any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
  02111-1307, USA.

=end
class CanvasSamplePrimitives < Gtk::VBox
  VERTICES = 10
  RADIUS = 60.0
  
  def initialize(aa)
    super(false, 4)
    @dragging = false
    self.border_width = 4
    show

    label = Gtk::Label.new("Drag an item with button 1. " +
                           "Click button 2 on an item to lower it, " +
                           "or button 3 to raise it.")
    pack_start(label, false, false, 0)
    label.show

    hbox = Gtk::HBox.new(false, 4)
    pack_start(hbox, false, false, 0)
    hbox.show

    # Create the canvas
    canvas = Goo::Canvas.new
    canvas.automatic_bounds = true
    canvas.bounds_from_origin = false
    canvas.bounds_padding = 4.0
    canvas.background_color_rgb = 0xc3c3ff
    canvas.set_bounds(0, 0, 604, 454)

    # Scale
    label = Gtk::Label.new("Scale:")
    hbox.pack_start(label, false, false, 0)
    label.show

    adjustment = Gtk::Adjustment.new(1.00, 0.05, 50.00, 0.05, 0.50, 0.50)
    button = Gtk::SpinButton.new(adjustment, 0.0, 2)
    adjustment.signal_connect('value_changed') do |adj,|
      canvas.scale = adj.value
    end
    button.set_size_request(50, -1)
    hbox.pack_start(button, false, false, 0)
    button.show

    label = Gtk::Label.new("Scale X:")
    hbox.pack_start(label, false, false, 0)
    label.show

    adjustment = Gtk::Adjustment.new(1.00, 0.05, 50.00, 0.05, 0.50, 0.50)
    button = Gtk::SpinButton.new(adjustment, 0.0, 2)
    adjustment.signal_connect('value_changed') do |adj,|
      canvas.scale_x = adj.value
    end
    button.set_size_request(50, -1)
    hbox.pack_start(button, false, false, 0)
    button.show

    label = Gtk::Label.new("Scale Y:")
    hbox.pack_start(label, false, false, 0)
    label.show

    adjustment = Gtk::Adjustment.new(1.00, 0.05, 50.00, 0.05, 0.50, 0.50)
    button = Gtk::SpinButton.new(adjustment, 0.0, 2)
    adjustment.signal_connect('value_changed') do |adj,|
      canvas.scale_y = adj.value
    end
    button.set_size_request(50, -1)
    hbox.pack_start(button, false, false, 0)
    button.show

    hbox = Gtk::HBox.new(false, 4)
    pack_start(hbox, false, false, 0)
    hbox.show

    # Center
    button = Gtk::CheckButton.new('Center scroll region')
    hbox.pack_start(button, false, false, 0)
    button.signal_connect("toggled") do |_button,|
      canvas.center_scroll_region = _button.active
    end
    # button.show

    # Move Ellipse
    button = Gtk::Button.new("Move Ellipse")
    hbox.pack_start(button, false, false, 0)
    button.show
    last_state = 0
    button.signal_connect("clicked") do |_button,|
      $ellipse2.transform = nil

      case last_state
      when 0
        $ellipse2.center_x = 300.0
        $ellipse2.center_y = 70.0
        $ellipse2.radius_x = 45.0
        $ellipse2.radius_y = 30.0
        $ellipse2.fill_color = 'red'
        $ellipse2.stroke_color = 'midnightblue'
        $ellipse2.line_width = 4.0
        $ellipse2.title = 'A red ellipse'
      when 1
        $ellipse2.center_x = 390.0
        $ellipse2.center_y = 150.0
        $ellipse2.radius_x = 45.0
        $ellipse2.radius_y = 40.0
        $ellipse2.fill_color = 'brown'
        $ellipse2.stroke_color = 'midnightblue'
        $ellipse2.line_width = 4.0
        $ellipse2.title = 'A brown ellipse'
      when 2
        $ellipse2.center_x = 0.0
        $ellipse2.center_y = 0.0
        $ellipse2.radius_y = 30.0
        $ellipse2.set_simple_transform(100, 100, 1, 0)
      when 3
        $ellipse2.set_simple_transform(200, 100, 2, 0)
      when 4
        $ellipse2.set_simple_transform(200, 200, 1, 45)
      when 5
        $ellipse2.set_simple_transform(50, 50, 0.2, 225)
      else
        $ellipse2.center_x = 335.0
        $ellipse2.center_y = 70.0
        $ellipse2.radius_x = 45.0
        $ellipse2.radius_y = 30.0
        $ellipse2.fill_color = 'purple'
        $ellipse2.stroke_color = 'midnightblue'
        $ellipse2.line_width = 4.0
        $ellipse2.title = 'A purple ellipse'
      end
      last_state = (last_state + 1) % 7
    end

    # Animate Ellipse
    button = Gtk::Button.new("Animate Ellipse")
    hbox.pack_start(button, false, false, 0)
    button.show
    button.signal_connect('clicked') do
      $ellipse2.animate(100, 100, 1, 90, true,
                        1000, 40, Goo::CanvasItem::ANIMATE_BOUNCE)
    end

    # Stop Animation
    button = Gtk::Button.new("Stop Animation")
    hbox.pack_start(button, false, false, 0)
    button.show
    button.signal_connect('clicked') do
      $ellipse2.stop_animation
    end

    # Create PDF
    button = Gtk::Button.new("Write PDF")
    hbox.pack_start(button, false, false, 0)
    button.show
    button.signal_connect('clicked') do
      puts "In write_pdf_clicked"

      Cairo::PDFSurface.new("demo.pdf", 9 * 72, 10 * 72) do |surface|
        Cairo::Context.new(surface) do |context|

          # Place it in the middle of our 9x10 page.
          context.translate(20, 130)

          canvas.render(context, nil, 1.0)

          context.show_page
        end
      end
    end

    button = Gtk::Button.new("Change Bounds")
    hbox.pack_start(button, false, false, 0)
    button.show
    bounds = [
              [-100, -100, 200, 200],
              [100, 100, 200, 200],
              [0, 0, 300, 300],
              [0, 0, 604, 454],
             ]
    bounds_n = 0
    button.signal_connect("clicked") do |_button,|
      if bounds_n < 4
        automatic_bounds = false
        bounds_from_origin = false
        canvas.set_bounds(*bounds[bounds_n])
      elsif bounds_n == 4
        automatic_bounds = true
        bounds_from_origin = true
      end

      canvas.automatic_bounds = automatic_bounds
      canvas.bounds_from_origin = bounds_from_origin
      canvas.bounds_padding = 50
      bounds_n = (bounds_n + 1) % 6
    end

    hbox = Gtk::HBox.new(false, 4)
    pack_start(hbox, false, false, 0)
    hbox.show

    # Scroll to
    label = Gtk::Label.new("Scroll To:")
    hbox.pack_start(label, false, false, 0)
    label.show

    button = Gtk::Button.new("50,50")
    hbox.pack_start(button, false, false, 0)
    button.show
    button.signal_connect('clicked') {canvas.scroll_to(50, 50)}

    button = Gtk::Button.new("250,250")
    hbox.pack_start(button, false, false, 0)
    button.show
    button.signal_connect('clicked') {canvas.scroll_to(250, 250)}

    button = Gtk::Button.new("500,500")
    hbox.pack_start(button, false, false, 0)
    button.show
    button.signal_connect('clicked') {canvas.scroll_to(500, 500)}

    # Scroll anchor
    label = Gtk::Label.new("Anchor:")
    hbox.pack_start(label, false, false, 0)
    label.show

    group = nil
    [
     ['NW', Gtk::ANCHOR_NW],
     ['NE', Gtk::ANCHOR_NE],
     ['W', Gtk::ANCHOR_W],
     ['C', Gtk::ANCHOR_CENTER],
     ['E', Gtk::ANCHOR_E],
     ['SW', Gtk::ANCHOR_SW],
     ['S', Gtk::ANCHOR_S],
     ['SE', Gtk::ANCHOR_SE],
    ].each do |name, anchor|
      if group
        button = Gtk::RadioButton.new(group, name)
      else
        button = Gtk::RadioButton.new(name)
      end
      group, = button.group
      hbox.pack_start(button, false, false, 0)
      button.show
      button.signal_connect('toggled') do |_button|
        canvas.anchor = anchor if _button.active?
      end
    end

    # Layout the stuff
    scrolled_window = Gtk::ScrolledWindow.new
    scrolled_window.show
    pack_start(scrolled_window, true, true, 0)

    canvas.show
    scrolled_window.add(canvas)

    # Add all the canvas items.
    setup_canvas(canvas)
  end

  def image_path(name)
    File.join(File.dirname(__FILE__), name)
  end

  def setup_item_signals(item)
    dragging = false
    drag_x = nil
    drag_y = nil

    item.signal_connect('motion_notify_event') do |item, target, event|
      if dragging && (event.state & Gdk::Window::BUTTON1_MASK == Gdk::Window::BUTTON1_MASK)
        item.translate(event.x - drag_x, event.y - drag_y)
      end
      true
    end

    item.signal_connect('button_press_event') do |item, target, event|
      case event.button
      when 1
        if event.state & Gdk::Window::SHIFT_MASK == Gdk::Window::SHIFT_MASK
          item.parent.remove_child(item)
        else
          drag_x = event.x
          drag_y = event.y

          fleur = Gdk::Cursor.new(Gdk::Cursor::FLEUR)
          canvas = item.canvas
          canvas.pointer_grab(item, Gdk::Event::POINTER_MOTION_MASK | Gdk::Event::BUTTON_RELEASE_MASK, fleur, event.time)
          dragging = true
        end
      when 2
        item.lower(nil)
      when 3
        item.raise(nil)
      end
      true
    end
  
    item.signal_connect('button_release_event') do |item, target, event|
      canvas = item.canvas
      canvas.pointer_ungrab(item, event.time) if canvas
      dragging = false
      true
    end
  end
  
  def setup_heading(root, text, pos)
    x = (pos % 3) * 200 + 100
    y = (pos / 3) * 150 + 5
  
    item = Goo::CanvasText.new(root, text, x, y, -1, Gtk::ANCHOR_N, :font => "Sans 12")
    item.skew_y(30, x, y)
  end
  
  def setup_divisions(root)
    group = Goo::CanvasGroup.new(root, nil)
    group.translate(2, 2)
    
    item = Goo::CanvasRect.new(group, 0, 0, 600, 450, :line_width => 4.0)
    item = Goo::CanvasPolyline.new_line(group, 0, 150, 600, 150, :line_width => 4.0)
    item = Goo::CanvasPolyline.new_line(group, 0, 300, 600, 300, :line_width => 4.0)
    item = Goo::CanvasPolyline.new_line(group, 200, 0, 200, 450, :line_width => 4.0)
    item = Goo::CanvasPolyline.new_line(group, 400, 0, 400, 450, :line_width => 4.0)
    
    setup_heading(group, 'Rectangles', 0)
    setup_heading(group, 'Ellipses', 1)
    setup_heading(group, 'Texts', 2)
    setup_heading(group, 'Images', 3)
    setup_heading(group, 'Lines', 4)
    setup_heading(group, 'Polygons', 7)
  end
  
  # FIXME: I think we may need to check byte order.
  def create_stipple(color_name, stipple_data)
    color = Gdk::Color.parse(color_name)
    stipple_data[2] = stipple_data[14] = (color.red >> 8)
    stipple_data[1] = stipple_data[13] = (color.green >> 8)
    stipple_data[0] = stipple_data[12] = (color.blue >> 8)
    surface = Cairo::ImageSurface.new(stipple_data.pack('C*'), Cairo::FORMAT_ARGB32, 2, 2, 8)
    pattern = Cairo::SurfacePattern.new(surface)
    pattern.extend = Cairo::EXTEND_REPEAT
    
    return pattern
  end
  
  def setup_rectangles(root)
    stipple_data = [ 0, 0, 0, 255,   0, 0, 0, 0,   0, 0, 0, 0,     0, 0, 0, 255 ]
  
    item = Goo::CanvasRect.new(root, 20, 30, 50, 30,
                              :stroke_color => "red",
                              :line_width => 8.0)
    setup_item_signals(item)
    
    pattern = create_stipple("mediumseagreen", stipple_data)
    item = Goo::CanvasRect.new(root, 90, 40, 90, 60,
                              :fill_pattern => pattern,
                              :stroke_color => 'black',
                              :line_width => 4.0)
    setup_item_signals(item)
    
    item = Goo::CanvasRect.new(root, 10, 80, 70, 60, :fill_color => "steelblue")
    setup_item_signals(item)
    
    item = Goo::CanvasRect.new(root, 20, 90, 70, 60,
                              :fill_color_rgba => 0x3cb37180,
                              :stroke_color => "blue",
                              :line_width => 2.0)
    setup_item_signals(item)
    
    item = Goo::CanvasRect.new(root, 110, 80, 50, 30,
                              :radius_x => 20.0,
                              :radius_y => 10.0,
                              :stroke_color => "yellow",
                              :fill_color_rgba => 0x3cb3f180)
    setup_item_signals(item)
    
    #TODO
    #  item = goo_demo_item_new(root, 30, 20, 50, 30,
    #          :fill_color => "yellow")
    #  setup_item_signals(item)
  end

  def setup_ellipses(root)
    stipple_data = [ 0, 0, 0, 255,   0, 0, 0, 0,   0, 0, 0, 0,     0, 0, 0, 255 ]
    
    ellipse1 = Goo::CanvasEllipse.new(root, 245, 45, 25, 15,
                                  :stroke_color => 'goldenrod',
                                  :line_width => 8.0)
    setup_item_signals(ellipse1)
    
    $ellipse2 = Goo::CanvasEllipse.new(root, 335, 70, 45, 30,
                                     :fill_color => 'wheat',
                                     :stroke_color => 'midnightblue',
                                     :line_width => 4.0,
                                     :title => 'An ellipse')
    setup_item_signals($ellipse2)
    
    pattern = create_stipple('cadetblue', stipple_data)
    ellipse3 = Goo::CanvasEllipse.new(root, 245, 110, 35, 30,
                                      :fill_pattern => pattern,
                                      :stroke_color => 'black',
                                      :line_width => 1.0)
    setup_item_signals(ellipse3)
  end
  

    
  def polish_diamond(root)
    group = Goo::CanvasGroup.new(root,
            :line_width => 1.0,
            :line_cap => Cairo::LINE_CAP_ROUND)
      group.translate(270, 230)
      setup_item_signals(group)
    
      VERTICES.times do |i|
        a = 2.0 * Math::PI * i / VERTICES
        x1 = RADIUS * Math.cos(a)
        y1 = RADIUS * Math.sin(a)
    
        ((i + 1)...VERTICES).each do |j|
          a = 2.0 * Math::PI * j / VERTICES
      x2 = RADIUS * Math.cos(a)
      y2 = RADIUS * Math.sin(a)
      item = Goo::CanvasPolyline.new_line(group, x1, y1, x2, y2)
    end
  end
  end
  
  SCALE = 7.0
  
  def make_hilbert(root)
    hilbert = "urdrrulurulldluuruluurdrurddldrrruluurdrurddldrddlulldrdldrrurd".split(//)
    stipple_data = [ 0, 0, 0, 255,   0, 0, 0, 0,   0, 0, 0, 0,     0, 0, 0, 255 ]
  
    points = [ 340.0, 290.0 ]
  
    i = 0
    hilbert.each do |c|
      case c
      when 'u'
        points[i+2] = points[i+0]
        points[i+3] = points[i+1] - SCALE
      when 'd'
        points[i+2] = points[i+0]
        points[i+3] = points[i+1] + SCALE
      when 'l'
        points[i+2] = points[i+0] - SCALE
        points[i+3] = points[i+1]
      when 'r'
        points[i+2] = points[i+0] + SCALE
        points[i+3] = points[i+1]
      end
      i += 2
    end
  
    pattern = create_stipple("red", stipple_data)
    item = Goo::CanvasPolyline.new(root, false, points,
            :line_width => 4.0,
            :stroke_pattern => pattern,
            :line_cap => Cairo::LINE_CAP_SQUARE,
            :line_join => Cairo::LINE_JOIN_MITER)
    setup_item_signals(item)
  end
  
  def setup_lines(root)
    polish_diamond(root)
    make_hilbert(root)
  
    # Arrow tests
  
    polyline1 = Goo::CanvasPolyline.new(root, false, 
                 [ 340.0, 170.0,
                   340.0, 230.0,
                   390.0, 230.0,
                   390.0, 170.0,
                 ],
                 :stroke_color => 'midnightblue',
                 :line_width => 3.0,
                 :start_arrow => true,
                 :end_arrow => true,
                 :arrow_tip_length => 3.0,
                 :arrow_length => 4.0,
                 :arrow_width => 3.5)
    setup_item_signals(polyline1)
  
    polyline2 = Goo::CanvasPolyline.new(root, false,
                 [ 356.0, 180.0,
                   374.0, 220.0,
                 ] ,
                 :stroke_color => "blue",
                 :line_width => 1.0,
                 :start_arrow => true,
                 :end_arrow => true,
                 :arrow_tip_length => 5.0,
                 :arrow_length => 6.0,
                 :arrow_width => 6.0)
    setup_item_signals(polyline2)
  
    polyline3 = Goo::CanvasPolyline.new(root, false,
                 [ 356.0, 220.0,
                   374.0, 180.0,
                 ],
                 :stroke_color => "blue",
                 :line_width => 1.0,
                 :start_arrow => true,
                 :end_arrow => true,
                 :arrow_tip_length => 5.0,
                 :arrow_length => 6.0,
                 :arrow_width => 6.0)
    setup_item_signals(polyline3)
  
    # Test polyline without any coords.
    polyline4 = Goo::CanvasPolyline.new(root, false, [])
    setup_item_signals(polyline4)
  
    # Test polyline with 1 coord and arrows.
    polyline5 = Goo::CanvasPolyline.new(root, false,
                 [ 356.0, 220.0 ],
                 :start_arrow => true,
                 :end_arrow => true)
    setup_item_signals(polyline5)
  end
  
  def setup_polygons(root)
    stipple_data = [ 0, 0, 0, 255,   0, 0, 0, 0,   0, 0, 0, 0,     0, 0, 0, 255 ]
  
    points =
    [ 210.0, 320.0,
      210.0, 380.0,
      260.0, 350.0,
    ]
    pattern = create_stipple("blue", stipple_data)
    polyline1 = Goo::CanvasPolyline.new(root, true, points,
                 :line_width => 1.0,
                 :fill_pattern => pattern,
                 :stroke_color => "black")
    setup_item_signals(polyline1)
  
    points =
    [ 270.0, 330.0,
      270.0, 430.0,
      390.0, 430.0,
      390.0, 330.0,
      310.0, 330.0,
      310.0, 390.0,
      350.0, 390.0,
      350.0, 370.0,
      330.0, 370.0,
      330.0, 350.0,
      370.0, 350.0,
      370.0, 410.0,
      290.0, 410.0,
      290.0, 330.0,
    ]
    polyline2 = Goo::CanvasPolyline.new(root, true, points,
                 :fill_color => 'tan',
                 :stroke_color => 'black',
                 :line_width => 3.0)
    setup_item_signals(polyline2)
  end
  
  def make_anchor(root, x, y)
    transform = Cairo::Matrix.new(0.8, 0.2, -0.3, 0.5, x, y)
  
    group = Goo::CanvasGroup.new(root)
    group.translate(x, y)
    group.transform = transform
    $transforms ||= []
    $transforms << transform  

    item = Goo::CanvasRect.new(group, -2.5, -2.5, 4, 4, :line_width => 1.0)
    setup_item_signals(item)
  
    group
  end
  
  def setup_texts(root)
    stipple_data = [ 0, 0, 0, 255,   0, 0, 0, 0,   0, 0, 0, 0,     0, 0, 0, 255 ]

    pattern = create_stipple('blue', stipple_data)
    item = Goo::CanvasText.new(make_anchor(root, 420, 20),
              'Anchor NW', 0, 0, -1, Gtk::ANCHOR_NW,
              :font => 'Sans Bold 24',
              :fill_pattern => pattern)
    setup_item_signals(item)
  
    item = Goo::CanvasText.new(make_anchor(root, 470, 75),
              "Anchor center\nJustify center\nMultiline text\nb8bit text ÅÄÖåäö",
              0, 0, -1, Gtk::ANCHOR_CENTER,
              :font => "monospace bold 14",
              :alignment => Pango::ALIGN_CENTER,
              :fill_color => 'firebrick')
    setup_item_signals(item)
  
    textitem = Goo::CanvasText.new(make_anchor(root, 420, 240),
            "This is a very long paragraph that will need to be wrapped over several lines so we can see what happens to line-breaking as the view is zoomed in and out.",
            0, 0, 180, Gtk::ANCHOR_W,
            :font => 'Sans 12',
            :fill_color => 'goldenrod')
    setup_item_signals(textitem)
  
    textitem = Goo::CanvasText.new(root,
            "Ellipsized text.",
            20, 420, 115, Gtk::ANCHOR_W,
            :font => 'Sans 12',
            :fill_color => "blue",
            :ellipsize => Pango::ELLIPSIZE_END)
    setup_item_signals(textitem)
  end
  
  def setup_invisible_texts(root)
    Goo::CanvasText.new(root, "Visible above 0.8x", 500, 330, -1,
             Gtk::ANCHOR_CENTER,
             :visibility => Goo::CanvasItem::VISIBLE_ABOVE_THRESHOLD,
             :visibility_threshold => 0.8)
    Goo::CanvasRect.new(root, 410.5, 322.5, 180, 15,
             :line_width => 1.0,
             :visibility => Goo::CanvasItem::VISIBLE_ABOVE_THRESHOLD,
             :visibility_threshold => 0.8)
  
    Goo::CanvasText.new(root, "Visible above 1.5x", 500, 350, -1,
             Gtk::ANCHOR_CENTER,
             :visibility => Goo::CanvasItem::VISIBLE_ABOVE_THRESHOLD,
             :visibility_threshold => 1.5)
    Goo::CanvasRect.new(root, 410.5, 342.5, 180, 15,
             :line_width => 1.0,
             :visibility => Goo::CanvasItem::VISIBLE_ABOVE_THRESHOLD,
             :visibility_threshold => 1.5)
  
    Goo::CanvasText.new(root, "Visible above 3.0x", 500, 370, -1,
             Gtk::ANCHOR_CENTER,
             :visibility => Goo::CanvasItem::VISIBLE_ABOVE_THRESHOLD,
             :visibility_threshold => 3.0)
    Goo::CanvasRect.new(root, 410.5, 362.5, 180, 15,
             :line_width => 1.0,
             :visibility => Goo::CanvasItem::VISIBLE_ABOVE_THRESHOLD,
             :visibility_threshold => 3.0)
  
    # This should never be seen.
    Goo::CanvasText.new(root, "Always Invisible", 500, 390, -1,
             Gtk::ANCHOR_CENTER,
             :visibility => Goo::CanvasItem::INVISIBLE)
    Goo::CanvasRect.new(root, 410.5, 350.5, 180, 15,
             :line_width => 1.0,
             :visibility => Goo::CanvasItem::INVISIBLE)
  end
  
  def plant_flower(root, x, y, anchor)
    surface = Cairo::ImageSurface.from_png(image_path('flower.png'))
    w = surface.width
    h = surface.height
  
    pattern = Cairo::SurfacePattern.new(surface)
  
    image = Goo::CanvasImage.new(root, nil, x, y,
                                 :pattern => pattern,
                                 :width => w,
                                 :height => h)
    setup_item_signals(image)
  end
  
  def setup_images(root)
    im = Gdk::Pixbuf.new(image_path('toroid.png'))
    
    w = im.width
    h = im.height
    image = Goo::CanvasImage.new(root, im, 100.0 - w / 2, 225.0 - h / 2,
                                :width => w, :height => h)
    setup_item_signals(image)
  
    plant_flower(root,  20.0, 170.0, Gtk::ANCHOR_NW)
    plant_flower(root, 180.0, 170.0, Gtk::ANCHOR_NE)
    plant_flower(root,  20.0, 280.0, Gtk::ANCHOR_SW)
    plant_flower(root, 180.0, 280.0, Gtk::ANCHOR_SE)
  end
  
  def setup_canvas(canvas)
    root = canvas.root_item
    root.signal_connect("button_press_event") { 
        puts "background received 'button-press' signal"
    }
  
    # Setup canvas items
    setup_divisions(root)
    setup_rectangles(root)
    setup_ellipses(root)
    setup_lines(root)
    setup_polygons(root)
    setup_texts(root)
    setup_images(root)
    setup_invisible_texts(root)
  end
end
