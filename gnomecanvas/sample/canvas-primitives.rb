#  -*- indent-tabs-mode: nil -*-
class CanvasSamplePrimitives < Gtk::VBox
  private

  def item_event(item, event)
    case event.event_type
    when Gdk::Event::BUTTON_PRESS
      item_x, item_y = item.parent.w2i(event.x, event.y)
      case event.button
      when 1
        if event.state & Gdk::Window::SHIFT_MASK == Gdk::Window::SHIFT_MASK
          item.destroy()
        else
          @x = item_x;
          @y = item_y;
          fleur = Gdk::Cursor.new(Gdk::Cursor::FLEUR)
          item.grab(Gdk::Event::POINTER_MOTION_MASK | Gdk::Event::BUTTON_RELEASE_MASK,
                    fleur,
                    event.time)
          @dragging = true
        end
      when 2
        if event.state & Gdk::Window::SHIFT_MASK == Gdk::Window::SHIFT_MASK
          item.lower_to_bottom()
        else
          item.lower(1)
        end
      when 3
        if event.state & Gdk::Window::SHIFT_MASK == Gdk::Window::SHIFT_MASK
          item.raise_to_top()
        else
          item.raise(1)
        end
      end
    when Gdk::Event::MOTION_NOTIFY
      item_x, item_y = item.parent.w2i(event.x, event.y)
      if @dragging && (event.state & Gdk::Window::BUTTON1_MASK == Gdk::Window::BUTTON1_MASK)
        item.move(item_x - @x, item_y - @y)
        @x = item_x;
        @y = item_y;
      end
    when Gdk::Event::BUTTON_RELEASE
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
    Gnome::CanvasText.new(root,
                          {:text => text,
                            :x => (pos % 3) * 200 + 100,
                            :y => (pos / 3) * 150 + 5,
                            :font => "Sans 12",
                            :anchor => Gtk::ANCHOR_N,
                            :fill_color => "black"})
  end

  def setup_divisions(root)
    group = Gnome::CanvasGroup.new(root,
                                   {:x => 0.0,
                                     :y => 0.0})
    setup_item(group)

    Gnome::CanvasRect.new(group,
                          {:x1 => 0.0,
                            :y1 => 0.0,
                            :x2 => 600.0,
                            :y2 => 450.0,
                            :outline_color => "black",
                            :width_units => 4.0})
    Gnome::CanvasLine.new(group,
                          {:points => [[0.0, 150.0], [600.0, 150.0]],
                            :fill_color => "black",
                            :width_units => 4.0})
    Gnome::CanvasLine.new(group,
                          {:points => [[0.0, 300.0], [600.0, 300.0]],
                            :fill_color => "black",
                            :width_units => 4.0})
    Gnome::CanvasLine.new(group,
                          {:points => [[200.0, 0.0], [200.0, 450.0]],
                            :fill_color => "black",
                            :width_units => 4.0})
    Gnome::CanvasLine.new(group,
                          {:points => [[400.0, 0.0], [400.0, 450.0]],
                            :fill_color => "black",
                            :width_units => 4.0})

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
    setup_item(Gnome::CanvasRect.new(root,
                                     {:x1 => 20.0,
                                       :y1 => 30.0,
                                       :x2 => 70.0,
                                       :y2 => 60.0,
                                       :outline_color => "red",
                                       :width_pixels => 8}))
    if root.canvas.aa?
      setup_item(Gnome::CanvasRect.new(root,
                                       {:x1 => 90.0,
                                         :y1 => 40.0,
                                         :x2 => 180.0,
                                         :y2 => 100.0,
                                         :fill_color_rgba => 0x3cb37180,
                                         :outline_color => "black",
                                         :width_units => 4.0}))
    else
      stipple = Gdk::Bitmap.create_from_data(nil, Gray50_Bits, Gray50_Width, Gray50_Height)
      setup_item(Gnome::CanvasRect.new(root,
                                       {:x1 => 90.0,
                                         :y1 => 40.0,
                                         :x2 => 180.0,
                                         :y2 => 100.0,
                                         :fill_color => "mediumseagreen",
                                         :fill_stipple => stipple,
                                         :outline_color => "black",
                                         :width_units => 4.0}))
    end

    setup_item(Gnome::CanvasRect.new(root,
                                     {:x1 => 10.0,
                                       :y1 => 80.0,
                                       :x2 => 80.0,
                                       :y2 => 140.0,
                                       :fill_color => "steelblue"}))
  end

  def setup_ellipses(root)
    setup_item(Gnome::CanvasEllipse.new(root,
                                        {:x1 => 220.0,
                                          :y1 => 30.0,
                                          :x2 => 270.0,
                                          :y2 => 60.0,
                                          :outline_color => "goldenrod",
                                          :width_pixels => 8}))
    setup_item(Gnome::CanvasEllipse.new(root,
                                        {:x1 => 290.0,
                                          :y1 => 40.0,
                                          :x2 => 380.0,
                                          :y2 => 100.0,
                                          :fill_color => "wheat",
                                          :outline_color => "midnightblue",
                                          :width_units => 4.0}))
    if root.canvas.aa?
      setup_item(Gnome::CanvasEllipse.new(root,
                                          {:x1 => 210.0,
                                            :y1 => 80.0,
                                            :x2 => 280.0,
                                            :y2 => 140.0,
                                            :fill_color_rgba => 0x5f9ea080,
                                            :outline_color => "black",
                                            :width_pixels => 0}))
    else
      stipple = Gdk::Bitmap.create_from_data(nil, Gray50_Bits, Gray50_Width, Gray50_Height)
      setup_item(Gnome::CanvasEllipse.new(root,
                                          {:x1 => 210.0,
                                            :y1 => 80.0,
                                            :x2 => 280.0,
                                            :y2 => 140.0,
                                            :fill_color => "cadetblue",
                                            :fill_stipple => stipple,
                                            :outline_color => "black",
                                            :width_pixels => 0}))
    end
  end

  def make_anchor(root, x, y)
    group = Gnome::CanvasGroup.new(root,
                                   {:x => x,
                                     :y => y})
    setup_item(group)
    Gnome::CanvasRect.new(group,
                          {:x1 => -2.0,
                            :y1 => -2.0,
                            :x2 => 2.0,
                            :y2 => 2.0,
                            :outline_color => "black",
                            :width_pixels => 1})
    group
  end

  def setup_texts(root)
    if root.canvas.aa?
      Gnome::CanvasText.new(make_anchor(root, 420.0, 20.0),
                            {:text => "Anchor NW",
                              :x => 0.0,
                              :y => 0.0,
                              :font => "Sans Bold 24",
                              :anchor => Gtk::ANCHOR_NW,
                              :fill_color_rgba => 0x0000ff80})
    else
      stipple = Gdk::Bitmap.create_from_data(nil, Gray50_Bits, Gray50_Width, Gray50_Height)
      Gnome::CanvasText.new(make_anchor(root, 420.0, 20.0),
                            {:text => "Anchor NW",
                              :x => 0.0,
                              :y => 0.0,
                              :font => "Sans Bold 24",
                              :anchor => Gtk::ANCHOR_NW,
                              :fill_color => "blue",
                              :fill_stipple => stipple})
    end

    Gnome::CanvasText.new(make_anchor(root, 470.0, 75.0),
                          {:text => "Anchor center\nJustify center\nMultiline text",
                            :x => 0.0,
                            :y => 0.0,
                            :font => "monospace bold 14",
                            :anchor => Gtk::ANCHOR_CENTER,
                            :justification => Gtk::JUSTIFY_CENTER,
                            :fill_color => "firebrick"})

    Gnome::CanvasText.new(make_anchor(root, 590.0, 140.0),
                          {:text => "Clipped text\nClipped text\nClipped text\nClipped text\nClipped text\nClipped text",
                            :x => 0.0,
                            :y => 0.0,
                            :font => "Sans 12",
                            :anchor => Gtk::ANCHOR_SE,
                            :clip => true,
                            :clip_width => 50.0,
                            :clip_height => 55.0,
                            :x_offset => 10.0,
                            :fill_color => "darkgreen"})
  end

  def plant_flower(root, x, y, anchor, aa)
    im = Gdk::Pixbuf.new("flower.png")

    unless im.nil?
      image = Gnome::CanvasPixbuf.new(root,
                                      :pixbuf => im,
                                      :x => x,
                                      :y => y,
                                      :width => im.width,
                                      :height => im.height,
                                      :anchor => anchor)
      setup_item(image)
      image.signal_connect("destroy", im) do |item, im|
      end
    end
  end

  def setup_images(root, aa)
    im = Gdk::Pixbuf.new("toroid.png")
    unless im.nil?
      image = Gnome::CanvasPixbuf.new(root,
                                      {:pixbuf => im,
                                        :x => 100.0,
                                        :y => 225.0,
                                        :width => im.width,
                                        :height => im.height,
                                        :anchor => Gtk::ANCHOR_CENTER})
      setup_item(image)
      image.signal_connect("destroy", im) do |item, im|
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

    group = Gnome::CanvasGroup.new(root,
                                   {:x => 270.0,
                                     :y => 230.0})
    setup_item(group)

    points = []

    0.upto(VERTICES - 1) do |i|
      a = 2.0 * Math::PI * i / VERTICES
      points[0] = [RADIUS * Math::cos(a), RADIUS * Math::sin(a)]

      (i + 1).upto(VERTICES - 1) do |j|
        a = 2.0 * Math::PI * j / VERTICES
        points[1] = [RADIUS * Math::cos(a), RADIUS * Math::sin(a)]
        Gnome::CanvasLine.new(group,
                              {:points => points,
                                :fill_color => "black",
                                :width_units => 1.0,
                                :cap_style => Gdk::GC::CAP_ROUND})
      end
    end
  end

  SCALE = 7.0
  def make_hilbert(root)
    hilbert = "urdrrulurulldluuruluurdrurddldrrruluurdrurddldrddlulldrdldrrurd"

    points = []
    points << [340.0, 290.0]

    hilbert.split(//).each_with_index do |c, i|
      case c
      when 'u'
        points << [points.last[0], points.last[1] - SCALE]
      when 'd'
        points << [points.last[0], points.last[1] + SCALE]
      when 'l'
        points << [points.last[0] - SCALE, points.last[1]]
      when 'r'
        points << [points.last[0] + SCALE, points.last[1]]
      end
    end

    if root.canvas.aa?
      setup_item(Gnome::CanvasLine.new(root,
                                       {:points => points,
                                         :fill_color_rgba => 0xff000080,
                                         :width_units => 4.0,
                                         :cap_style => Gdk::GC::CAP_PROJECTING,
                                         :join_style => Gdk::GC::JOIN_MITER}))
    else
      stipple = Gdk::Bitmap.create_from_data(nil, Gray50_Bits, Gray50_Width, Gray50_Height)
      setup_item(Gnome::CanvasLine.new(root,
                                       {:points => points,
                                         :fill_color => "red",
                                         :fill_stipple => stipple,
                                         :width_units => 4.0,
                                         :cap_style => Gdk::GC::CAP_PROJECTING,
                                         :join_style => Gdk::GC::JOIN_MITER}))
    end
  end

  def setup_lines(root)
    polish_diamond (root)
    make_hilbert (root)

    # Arrow tests
    points = [[340.0, 170.0], [340.0, 230.0], [390.0, 230.0], [390.0, 170.0]]
    setup_item(Gnome::CanvasLine.new(root,
                                     {:points => points,
                                       :fill_color => "midnightblue",
                                       :width_units => 3.0,
                                       :first_arrowhead => true,
                                       :last_arrowhead => true,
                                       :arrow_shape_a => 8.0,
                                       :arrow_shape_b => 12.0,
                                       :arrow_shape_c => 4.0}))

    points = [[356.0, 180.0], [374.0, 220.0]]
    setup_item(Gnome::CanvasLine.new(root,
                                     {:points => points,
                                       :fill_color => "blue",
                                       :width_pixels => 0,
                                       :first_arrowhead => true,
                                       :last_arrowhead => true,
                                       :arrow_shape_a => 6.0,
                                       :arrow_shape_b => 6.0,
                                       :arrow_shape_c => 4.0}))

    points = [[356.0, 220.0], [374.0, 180.0]]
    setup_item(Gnome::CanvasLine.new(root,
                                     {:points => points,
                                       :fill_color => "blue",
                                       :width_pixels => 0,
                                       :first_arrowhead => true,
                                       :last_arrowhead => true,
                                       :arrow_shape_a => 6.0,
                                       :arrow_shape_b => 6.0,
                                       :arrow_shape_c => 4.0}))
  end

  def setup_curves(root)
    path_def = Gnome::CanvasPathDef.new()
    path_def.moveto(500.0, 175.0)
    path_def.curveto(550.0, 175.0, 550.0, 275.0, 500.0, 275.0)
    setup_item(Gnome::CanvasBpath.new(root,
                                      {:bpath => path_def,
                                        :outline_color => "black",
                                        :width_pixels => 4}))
  end

  def setup_polygons(root)
    points = [[210.0, 320.0], [210.0, 380.0], [260.0, 350.0]]
    if root.canvas.aa?
      setup_item(Gnome::CanvasPolygon.new(root,
                                          {:points => points,
                                            :fill_color_rgba => 0x0000ff80,
                                            :outline_color => "black"}))
    else
      stipple = Gdk::Bitmap.create_from_data(nil, Gray50_Bits, Gray50_Width, Gray50_Height)
      setup_item(Gnome::CanvasPolygon.new(root,
                                          {:points => points,
                                            :fill_color => "blue",
                                            :fill_stipple => stipple,
                                            :outline_color => "black"}))
    end

    points = []
    points << [270.0, 330.0]
    points << [270.0, 430.0]
    points << [390.0, 430.0]
    points << [390.0, 330.0]
    points << [310.0, 330.0]
    points << [310.0, 390.0]
    points << [350.0, 390.0]
    points << [350.0, 370.0]
    points << [330.0, 370.0]
    points << [330.0, 350.0]
    points << [370.0, 350.0]
    points << [370.0, 410.0]
    points << [290.0, 410.0]
    points << [290.0, 330.0]
    setup_item(Gnome::CanvasPolygon.new(root,
                                        {:points => points,
                                          :fill_color => "tan",
                                          :outline_color => "black",
                                          :width_units => 3.0}))
  end

  def setup_widgets(root)
    w = Gtk::Button.new("Hello world!")
    setup_item(Gnome::CanvasWidget.new(root,
                                       {:widget => w,
                                         :x => 420.0,
                                         :y => 330.0,
                                         :width => 100.0,
                                         :height => 40.0,
                                         :anchor => Gtk::ANCHOR_NW,
                                         :size_pixels => FALSE}))
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
    canvas = Gnome::Canvas.new(aa)

    # Setup canvas items
    root = canvas.root

    setup_divisions(root)
    setup_rectangles(root)
    setup_ellipses(root)
    setup_texts(root)
    setup_images(root, aa)
    setup_lines(root)
    setup_curves(root)
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
    w.set_size_request(50, -1)
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

    canvas.set_size_request(600, 450)
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
