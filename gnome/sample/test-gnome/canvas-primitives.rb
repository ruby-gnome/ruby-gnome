class CanvasSamplePrimitives < Gtk::VBox
  private

  def item_event(item, event)
    case event.event_type
    when Gdk::BUTTON_PRESS
      item_x, item_y = item.parent.w2i(event.x, event.y)
      case event.button
      when 1
	if event.state & Gdk::SHIFT_MASK == Gdk::SHIFT_MASK
	  item.destroy()
	else
	  @x = item_x;
	  @y = item_y;
	  #TODO? fleur = Gdk::Cursor.new(Gdk::Cursor::FLEUR)
	  fleur = nil
	  item.grab(Gdk::POINTER_MOTION_MASK | Gdk::BUTTON_RELEASE_MASK,
		    fleur,
		    event.time)
	  #TODO? fleur.destroy()
	  @dragging = true
	end
      when 2
	if event.state & Gdk::SHIFT_MASK == Gdk::SHIFT_MASK
	  item.lower_to_bottom()
	else
	  item.lower(1)
	end
      when 3
	if event.state & Gdk::SHIFT_MASK == Gdk::SHIFT_MASK
	  item.raise_to_top()
	else
	  item.raise(1)
	end
      end
    when Gdk::MOTION_NOTIFY
      item_x, item_y = item.parent.w2i(event.x, event.y)
      if @dragging && (event.state & Gdk::BUTTON1_MASK == Gdk::BUTTON1_MASK)
	item.move(item_x - @x, item_y - @y)
	@x = item_x;
	@y = item_y;
      end
    when Gdk::BUTTON_RELEASE
      item.ungrab(event.time)
      @dragging = FALSE;
    end
  end

  def setup_item(item)
    item.signal_connect("event") do |item, event|
      item_event(item, event)
    end
  end

  def setup_heading(root, text, pos)
    root.item_new(Gnome::CanvasText,
		  "text", text,
		  "x", (pos % 3) * 200 + 100,
		  "y", (pos / 3) * 150 + 5,
		  "font", "-adobe-helvetica-medium-r-normal--12-*-72-72-p-*-iso8859-1",
		  "anchor", Gtk::ANCHOR_N,
		  "fill_color", "black")
  end

  def setup_divisions(root)
    group = root.item_new(Gnome::CanvasGroup,
			  "x", 0.0,
			  "y", 0.0)
    setup_item(group)

    group.item_new(Gnome::CanvasRect,
		   "x1", 0.0,
		   "y1", 0.0,
		   "x2", 600.0,
		   "y2", 450.0,
		   "outline_color", "black",
		   "width_units", 4.0)
    points = Gnome::CanvasPoints.new(2)
    points[0] = 0.0
    points[1] = 150.0
    points[2] = 600.0
    points[3] = 150.0
    group.item_new(Gnome::CanvasLine,
		   "points", points,
		   "fill_color", "black",
		   "width_units", 4.0)

    points[0] = 0.0
    points[1] = 300.0
    points[2] = 600.0
    points[3] = 300.0
    group.item_new(Gnome::CanvasLine,
		   "points", points,
		   "fill_color", "black",
		   "width_units", 4.0)

    points[0] = 200.0
    points[1] = 0.0
    points[2] = 200.0
    points[3] = 450.0
    group.item_new(Gnome::CanvasLine,
		   "points", points,
		   "fill_color", "black",
		   "width_units", 4.0)

    points[0] = 400.0
    points[1] = 0.0
    points[2] = 400.0
    points[3] = 450.0
    group.item_new(Gnome::CanvasLine,
		   "points", points,
		   "fill_color", "black",
		   "width_units", 4.0)

    setup_heading(group, "Rectangles", 0)
    setup_heading(group, "Ellipses", 1)
    setup_heading(group, "Texts", 2)
    setup_heading(group, "Images", 3)
    setup_heading(group, "Lines", 4)
    setup_heading(group, "Curves", 5)
    setup_heading(group, "Arcs", 6)
    setup_heading(group, "Polygons", 7)
    setup_heading(group, "Widgets", 8)
  end

  Gray50_Width = 2
  Gray50_Height = 2
  Gray50_Bits = [0x02, 0x01].pack("CC")

  def setup_rectangles(root)
    setup_item(root.item_new(Gnome::CanvasRect,
			     "x1", 20.0,
			     "y1", 30.0,
			     "x2", 70.0,
			     "y2", 60.0,
			     "outline_color", "red",
			     "width_pixels", 8))
    if root.canvas.aa?
      setup_item(root.item_new(Gnome::CanvasRect,
			       "x1", 90.0,
			       "y1", 40.0,
			       "x2", 180.0,
			       "y2", 100.0,
			       "fill_color_rgba", 0x3cb37180,
			       "outline_color", "black",
			       "width_units", 4.0))
    else
      stipple = Gdk::Bitmap.create_from_data(nil, Gray50_Bits, Gray50_Width, Gray50_Height)
      setup_item(root.item_new(Gnome::CanvasRect,
			       "x1", 90.0,
			       "y1", 40.0,
			       "x2", 180.0,
			       "y2", 100.0,
			       "fill_color", "mediumseagreen",
			       "fill_stipple", stipple,
			       "outline_color", "black",
			       "width_units", 4.0))
      #TODO? stipple.unref()
    end

    setup_item(root.item_new(Gnome::CanvasRect,
			     "x1", 10.0,
			     "y1", 80.0,
			     "x2", 80.0,
			     "y2", 140.0,
			     "fill_color", "steelblue"))
  end

  def setup_ellipses(root)
    setup_item(root.item_new(Gnome::CanvasEllipse,
			     "x1", 220.0,
			     "y1", 30.0,
			     "x2", 270.0,
			     "y2", 60.0,
			     "outline_color", "goldenrod",
			     "width_pixels", 8))
    setup_item(root.item_new(Gnome::CanvasEllipse,
			     "x1", 290.0,
			     "y1", 40.0,
			     "x2", 380.0,
			     "y2", 100.0,
			     "fill_color", "wheat",
			     "outline_color", "midnightblue",
			     "width_units", 4.0))
    if root.canvas.aa?
      setup_item(root.item_new(Gnome::CanvasEllipse,
			       "x1", 210.0,
			       "y1", 80.0,
			       "x2", 280.0,
			       "y2", 140.0,
			       "fill_color_rgba", 0x5f9ea080,
			       "outline_color", "black",
			       "width_pixels", 0))
    else
      stipple = Gdk::Bitmap.create_from_data(nil, Gray50_Bits, Gray50_Width, Gray50_Height)
      setup_item(root.item_new(Gnome::CanvasEllipse,
			       "x1", 210.0,
			       "y1", 80.0,
			       "x2", 280.0,
			       "y2", 140.0,
			       "fill_color", "cadetblue",
			       "fill_stipple", stipple,
			       "outline_color", "black",
			       "width_pixels", 0))
      #TODO? stipple.unref()
    end
  end

  def make_anchor(root, x, y)
    group = root.item_new(Gnome::CanvasGroup,
			  "x", x,
			  "y", y)
    setup_item(group)
    group.item_new(Gnome::CanvasRect,
		   "x1", -2.0,
		   "y1", -2.0,
		   "x2", 2.0,
		   "y2", 2.0,
		   "outline_color", "black",
		   "width_pixels", 1)
    group
  end

  def setup_texts(root)
    if root.canvas.aa?
      make_anchor(root, 420.0, 20.0).item_new(Gnome::CanvasText,
					      "text", "Anchor NW",
					      "x", 0.0,
					      "y", 0.0,
					      "font", "-adobe-helvetica-bold-r-normal--24-240-75-75-p-138-iso8859-1",
					      "anchor", Gtk::ANCHOR_NW,
					      "fill_color_rgba", 0x0000ff80)
    else
      stipple = Gdk::Bitmap.create_from_data(nil, Gray50_Bits, Gray50_Width, Gray50_Height)
      make_anchor(root, 420.0, 20.0).item_new(Gnome::CanvasText,
					      "text", "Anchor NW",
					      "x", 0.0,
					      "y", 0.0,
					      "font", "-adobe-helvetica-bold-r-normal--24-240-75-75-p-138-iso8859-1",
					      "anchor", Gtk::ANCHOR_NW,
					      "fill_color", "blue",
					      "fill_stipple", stipple)
      #TODO? stipple.unref()
    end

    make_anchor(root, 470.0, 75.0).item_new(Gnome::CanvasText,
					    "text", "Anchor center\nJustify center\nMultiline text",
					    "x", 0.0,
					    "y", 0.0,
					    "font", "-b&h-lucida-bold-r-normal-*-14-*-*-*-p-*-iso8859-1",
					    "anchor", Gtk::ANCHOR_CENTER,
					    "justification", Gtk::JUSTIFY_CENTER,
					    "fill_color", "firebrick")

    make_anchor(root, 590.0, 140.0).item_new(Gnome::CanvasText,
					     "text", "Clipped text\nClipped text\nClipped text\nClipped text\nClipped text\nClipped text",
					     "x", 0.0,
					     "y", 0.0,
					     "font", "-*-clean-medium-r-*-*-12-*-*-*-*-*-*-*",
					     "anchor", Gtk::ANCHOR_SE,
					     "clip", TRUE,
					     "clip_width", 50.0,
					     "clip_height", 55.0,
					     "x_offset", 10.0,
					     "fill_color", "darkgreen")
  end

  def plant_flower(root, x, y, anchor, aa)
    if aa
      im = Gnome::Canvas.load_alpha("flower.png")
    else
      im = GdkImlib::Image.new("flower.png")
    end

    unless im.nil?
      image = root.item_new(Gnome::CanvasImage,
			    "image", im,
			    "x", x,
			    "y", y,
			    "width", im.rgb_width,
			    "height", im.rgb_height,
			    "anchor", anchor)
      setup_item(image)
      image.signal_connect("destroy", im) do |item, im|
	#TODO? im.destroy_image()
      end
    end
  end

  def setup_images(root, aa)
    if aa
      im = Gnome::Canvas.load_alpha("toroid.png")
    else
      im = GdkImlib::Image.new("toroid.png")
    end
    unless im.nil?
      image = root.item_new(Gnome::CanvasImage,
			    "image", im,
			    "x", 100.0,
			    "y", 225.0,
			    "width", im.rgb_width,
			    "height", im.rgb_height,
			    "anchor", Gtk::ANCHOR_CENTER)
      setup_item(image)
      image.signal_connect("destroy", im) do |item, im|
	#TODO? im.destroy_image()
      end
    end
    plant_flower (root,  20.0, 170.0, Gtk::ANCHOR_NW, aa);
    plant_flower (root, 180.0, 170.0, Gtk::ANCHOR_NE, aa);
    plant_flower (root,  20.0, 280.0, Gtk::ANCHOR_SW, aa);
    plant_flower (root, 180.0, 280.0, Gtk::ANCHOR_SE, aa);
  end

  VERTICES = 10
  RADIUS = 60.0
  def polish_diamond(root)

    group = root.item_new(Gnome::CanvasGroup,
			  "x", 270.0,
			  "y", 230.0)
    setup_item(group)

    points = Gnome::CanvasPoints.new(2)

    0.upto(VERTICES - 1) do |i|
      a = 2.0 * Math::PI * i / VERTICES
      points[0] = RADIUS * Math::cos(a)
      points[1] = RADIUS * Math::sin(a)

      (i + 1).upto(VERTICES - 1) do |j|
	a = 2.0 * Math::PI * j / VERTICES
	points[2] = RADIUS * Math::cos(a)
	points[3] = RADIUS * Math::sin(a)
	group.item_new(Gnome::CanvasLine,
		       "points", points,
		       "fill_color", "black",
		       "width_units", 1.0,
		       "cap_style", Gdk::CAP_ROUND)
      end
    end
    points.free()
  end

  SCALE = 7.0
  def make_hilbert(root)
    hilbert = "urdrrulurulldluuruluurdrurddldrrruluurdrurddldrddlulldrdldrrurd"

    points = Gnome::CanvasPoints.new(hilbert.size + 1)
    points[0] = 340.0;
    points[1] = 290.0;

    hilbert.split(//).each_with_index do |c, i|
      case c
      when 'u'
	points[i * 2 + 2] = points[i * 2 + 0]
	points[i * 2 + 3] = points[i * 2 + 1] - SCALE
      when 'd'
	points[i * 2 + 2] = points[i * 2 + 0]
	points[i * 2 + 3] = points[i * 2 + 1] + SCALE
      when 'l'
	points[i * 2 + 2] = points[i * 2 + 0] - SCALE
	points[i * 2 + 3] = points[i * 2 + 1]
      when 'r'
	points[i * 2 + 2] = points[i * 2 + 0] + SCALE
	points[i * 2 + 3] = points[i * 2 + 1]
      end
    end

    if root.canvas.aa?
      setup_item(root.item_new(Gnome::CanvasLine,
			       "points", points,
			       "fill_color_rgba", 0xff000080,
			       "width_units", 4.0,
			       "cap_style", Gdk::CAP_PROJECTING,
			       "join_style", Gdk::JOIN_MITER))
    else
      stipple = Gdk::Bitmap.create_from_data(nil, Gray50_Bits, Gray50_Width, Gray50_Height)
      setup_item(root.item_new(Gnome::CanvasLine,
			       "points", points,
			       "fill_color", "red",
			       "fill_stipple", stipple,
			       "width_units", 4.0,
			       "cap_style", Gdk::CAP_PROJECTING,
			       "join_style", Gdk::JOIN_MITER))
      #TODO? stipple.unref()
    end

    points.free()
  end

  def setup_lines(root)
    polish_diamond (root)
    make_hilbert (root)

    # Arrow tests
    points = Gnome::CanvasPoints.new(4)
    points[0] = 340.0
    points[1] = 170.0
    points[2] = 340.0
    points[3] = 230.0
    points[4] = 390.0
    points[5] = 230.0
    points[6] = 390.0
    points[7] = 170.0
    setup_item(root.item_new(Gnome::CanvasLine,
			     "points", points,
			     "fill_color", "midnightblue",
			     "width_units", 3.0,
			     "first_arrowhead", TRUE,
			     "last_arrowhead", TRUE,
			     "arrow_shape_a", 8.0,
			     "arrow_shape_b", 12.0,
			     "arrow_shape_c", 4.0))
    points.free()

    points = Gnome::CanvasPoints.new(2)
    points[0] = 356.0
    points[1] = 180.0
    points[2] = 374.0
    points[3] = 220.0
    setup_item(root.item_new(Gnome::CanvasLine,
			     "points", points,
			     "fill_color", "blue",
			     "width_pixels", 0,
			     "first_arrowhead", TRUE,
			     "last_arrowhead", TRUE,
			     "arrow_shape_a", 6.0,
			     "arrow_shape_b", 6.0,
			     "arrow_shape_c", 4.0))

    points[0] = 356.0
    points[1] = 220.0
    points[2] = 374.0
    points[3] = 180.0
    setup_item(root.item_new(Gnome::CanvasLine,
			     "points", points,
			     "fill_color", "blue",
			     "width_pixels", 0,
			     "first_arrowhead", TRUE,
			     "last_arrowhead", TRUE,
			     "arrow_shape_a", 6.0,
			     "arrow_shape_b", 6.0,
			     "arrow_shape_c", 4.0))
    points.free()
  end

  def setup_polygons(root)
    points = Gnome::CanvasPoints.new(3)
    points[0] = 210.0
    points[1] = 320.0
    points[2] = 210.0
    points[3] = 380.0
    points[4] = 260.0
    points[5] = 350.0
    if root.canvas.aa?
      setup_item(root.item_new(Gnome::CanvasPolygon,
			       "points", points,
			       "fill_color_rgba", 0x0000ff80,
			       "outline_color", "black"))
    else
      stipple = Gdk::Bitmap.create_from_data(nil, Gray50_Bits, Gray50_Width, Gray50_Height)
      setup_item(root.item_new(Gnome::CanvasPolygon,
			       "points", points,
			       "fill_color", "blue",
			       "fill_stipple", stipple,
			       "outline_color", "black"))
      #TODO? stipple.unref()
    end
    points.free()

    points = Gnome::CanvasPoints.new(14)
    points[0] = 270.0
    points[1] = 330.0
    points[2] = 270.0
    points[3] = 430.0
    points[4] = 390.0
    points[5] = 430.0
    points[6] = 390.0
    points[7] = 330.0
    points[8] = 310.0
    points[9] = 330.0
    points[10] = 310.0
    points[11] = 390.0
    points[12] = 350.0
    points[13] = 390.0
    points[14] = 350.0
    points[15] = 370.0
    points[16] = 330.0
    points[17] = 370.0
    points[18] = 330.0
    points[19] = 350.0
    points[20] = 370.0
    points[21] = 350.0
    points[22] = 370.0
    points[23] = 410.0
    points[24] = 290.0
    points[25] = 410.0
    points[26] = 290.0
    points[27] = 330.0
    setup_item(root.item_new(Gnome::CanvasPolygon,
			     "points", points,
			     "fill_color", "tan",
			     "outline_color", "black",
			     "width_units", 3.0))
    points.free()
  end

  def setup_widgets(root)
    w = Gtk::Button.new("Hello world!")
    setup_item(root.item_new(Gnome::CanvasWidget,
			     "widget", w,
			     "x", 420.0,
			     "y", 330.0,
			     "width", 100.0,
			     "height", 40.0,
			     "anchor", Gtk::ANCHOR_NW,
			     "size_pixels", FALSE))
    w.show()
  end

  def key_press(canvas, event)
    x, y = canvas.get_scroll_offsets()

    case event.keyval
    when Gdk::GDK_Up
      canvas.scroll_to(x, y - 20)
    when Gdk::GDK_Down
      canvas.scroll_to(x, y + 20)
    when Gdk::GDK_Left
      canvas.scroll_to(x - 10, y)
    when Gdk::GDK_Right
      canvas.scroll_to(x + 10, y)
    end
  end

  def initialize(aa)
    super(false, 4)
    border_width = 4
    show()

    w = Gtk::Label.new(<<EOS)
Drag an item with button 1.  Click button 2 on an item to lower it,
or button 3 to raise it.  Shift+click with buttons 2 or 3 to send
an item to the bottom or top, respectively.
EOS
    pack_start(w, false, false, 0)
    w.show()

    hbox = Gtk::HBox.new(false, 4)
    pack_start(hbox, false, false, 0)
    hbox.show()

    # Create the canvas
    if aa
      canvas = Gnome::Canvas.new_aa()
    else
      canvas = Gnome::Canvas.new()
    end

    # Setup canvas items
    root = canvas.root

    setup_divisions(root)
    setup_rectangles(root)
    setup_ellipses(root)
    setup_texts(root)
    setup_images(root, aa)
    setup_lines(root)
    setup_polygons(root)
    setup_widgets(root)

    # Zoom

    w = Gtk::Label.new("Zoom:")
    hbox.pack_start(w, false, false, 0)
    w.show()

    adj = Gtk::Adjustment.new(1.00, 0.05, 5.00, 0.05, 0.50, 0.50)
    adj.signal_connect("value_changed") do
      canvas.set_pixels_per_unit(adj.value)
    end

    w = Gtk::SpinButton.new(adj, 0.0, 2)
    w.set_usize(50, 0)
    hbox.pack_start(w, false, false, 0)
    w.show()

    # Layout the stuff

    table = Gtk::Table.new(2, 2, false)
    table.set_row_spacings(4)
    table.set_col_spacings(4)
    pack_start(table, true, true, 0)
    table.show()

    frame = Gtk::Frame.new()
    frame.set_shadow_type(Gtk::SHADOW_IN);
    table.attach(frame,
		 0, 1, 0, 1,
		 Gtk::EXPAND | Gtk::FILL | Gtk::SHRINK,
		 Gtk::EXPAND | Gtk::FILL | Gtk::SHRINK,
		 0, 0)
    frame.show()

    canvas.set_usize(600, 450)
    canvas.set_scroll_region(0, 0, 600, 450);
    frame.add(canvas)
    canvas.show()

    canvas.signal_connect_after("key_press_event") do |item, event|
      key_press(item, event)
    end

    w = Gtk::HScrollbar.new(canvas.hadjustment)
    table.attach(w,
		 0, 1, 1, 2,
		 Gtk::EXPAND | Gtk::FILL | Gtk::SHRINK,
		 Gtk::FILL,
		 0, 0);
    w.show()

    w = Gtk::VScrollbar.new(canvas.vadjustment)
    table.attach(w,
		 1, 2, 0, 1,
		 Gtk::FILL,
		 Gtk::EXPAND | Gtk::FILL | Gtk::SHRINK,
		 0, 0);
    w.show()
    canvas.flags = Gtk::Widget::CAN_FOCUS
    canvas.grab_focus()
  end
end
