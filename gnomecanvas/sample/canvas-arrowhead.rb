#  -*- indent-tabs-mode: nil -*-
class CanvasSampleArrowhead < Gtk::VBox
  LEFT = 50.0
  RIGHT = 350.0
  MIDDLE = 150.0
  DEFAULT_WIDTH = 2
  DEFAULT_SHAPE_A = 8
  DEFAULT_SHAPE_B = 10
  DEFAULT_SHAPE_C = 3

  private

  def set_dimension(arrow, text, x1, y1, x2, y2, tx, ty, dim)
    arrow.set({:points => [[x1, y1], [x2, y2]]})
    text.set({:text => dim.to_s,
               :x => tx,
               :y => ty})
  end

  def move_drag_box(item, x, y)
    item.set({:x1 => x - 5.0,
               :y1 => y - 5.0,
               :x2 => x + 5.0,
               :y2 => y + 5.0})
  end

  def set_arrow_shape(canvas)
    # Big arrow
    @big_arrow.set({:width_pixels => 10 * @width,
                     :arrow_shape_a => @shape_a * 10,
                     :arrow_shape_b => @shape_b * 10,
                     :arrow_shape_c => @shape_c * 10})

    # Outline
    points = []
    points << [RIGHT - 10 * @shape_a, MIDDLE]
    points << [RIGHT - 10 * @shape_b, MIDDLE - 10 * (@shape_c + @width / 2.0)]
    points << [RIGHT, MIDDLE]
    points << [points[1][0], MIDDLE + 10 * (@shape_c + @width / 2.0)]
    points << [points[0][0], points[0][1]]
    @outline.set({:points => points})

    # Drag boxes
    move_drag_box(@width_drag_box, LEFT, MIDDLE - 10 * @width / 2.0)
    move_drag_box(@shape_a_drag_box, RIGHT - 10 * @shape_a, MIDDLE)
    move_drag_box(@shape_b_c_drag_box, RIGHT - 10 * @shape_b, MIDDLE - 10 * (@shape_c + @width / 2.0))

    #Dimensions
    set_dimension(@width_arrow, @width_text,
                  LEFT - 10,
                  MIDDLE - 10 * @width / 2.0,
                  LEFT - 10,
                  MIDDLE + 10 * @width / 2.0,
                  LEFT - 15,
                  MIDDLE,
                  @width);

    set_dimension(@shape_a_arrow, @shape_a_text,
                  RIGHT - 10 * @shape_a,
                  MIDDLE + 10 * (@width / 2.0 + @shape_c) + 10,
                  RIGHT,
                  MIDDLE + 10 * (@width / 2.0 + @shape_c) + 10,
                  RIGHT - 10 * @shape_a / 2.0,
                  MIDDLE + 10 * (@width / 2.0 + @shape_c) + 15,
                  @shape_a);

    set_dimension(@shape_b_arrow, @shape_b_text,
                  RIGHT - 10 * @shape_b,
                  MIDDLE + 10 * (@width / 2.0 + @shape_c) + 35,
                  RIGHT,
                  MIDDLE + 10 * (@width / 2.0 + @shape_c) + 35,
                  RIGHT - 10 * @shape_b / 2.0,
                  MIDDLE + 10 * (@width / 2.0 + @shape_c) + 40,
                  @shape_b);

    set_dimension(@shape_c_arrow, @shape_c_text,
                  RIGHT + 10,
                  MIDDLE - 10 * @width / 2.0,
                  RIGHT + 10,
                  MIDDLE - 10 * (@width / 2.0 + @shape_c),
                  RIGHT + 15,
                  MIDDLE - 10 * (@width / 2.0 + @shape_c / 2.0),
                  @shape_c);

    # Info
    @width_info.set({:text => @width.to_s})
    @shape_a_info.set({:text => @shape_a.to_s})
    @shape_b_info.set({:text => @shape_b.to_s})
    @shape_c_info.set({:text => @shape_c.to_s})

    # Sample arrows
    @sample_1.set({:width_pixels => @width,
                    :arrow_shape_a => @shape_a,
                    :arrow_shape_b => @shape_b,
                    :arrow_shape_c => @shape_c})
    @sample_2.set({:width_pixels => @width,
                    :arrow_shape_a => @shape_a,
                    :arrow_shape_b => @shape_b,
                    :arrow_shape_c => @shape_c})
    @sample_3.set({:width_pixels => @width,
                    :arrow_shape_a => @shape_a,
                    :arrow_shape_b => @shape_b,
                    :arrow_shape_c => @shape_c})
  end

  def create_drag_box(root)
    box = Gnome::CanvasRect.new(root,
                                {:fill_color => nil,
                                  :outline_color => "black",
                                  :width_pixels => 1})
    box.signal_connect("event") do |item, event|
      case event.event_type
      when Gdk::Event::ENTER_NOTIFY
        item.set({:fill_color => "red"})
      when Gdk::Event::LEAVE_NOTIFY
        if event.state & Gdk::Window::BUTTON1_MASK == 0
          item.set({:fill_color => nil})
        end
      when Gdk::Event::BUTTON_PRESS
        fleur = Gdk::Cursor.new(Gdk::Cursor::FLEUR)
        item.grab(Gdk::Event::POINTER_MOTION_MASK | Gdk::Event::BUTTON_RELEASE_MASK,
                  fleur,
                  event.time)
      when Gdk::Event::BUTTON_RELEASE
        item.ungrab(event.time)
      end
    end
    box.signal_connect("event", Proc.new()) do |item, event, proc|
      proc.call(item, event)
    end
    box
  end

  def width_event(item, event)
    return if event.event_type != Gdk::Event::MOTION_NOTIFY
    return if event.state & Gdk::Window::BUTTON1_MASK == 0
    width = (MIDDLE - event.y) / 5.0
    return if width < 0.0
    @width = width
    set_arrow_shape(item.canvas)
  end

  def shape_a_event(item, event)
    return if event.event_type != Gdk::Event::MOTION_NOTIFY
    return if event.state & Gdk::Window::BUTTON1_MASK == 0
    shape_a = (RIGHT - event.x) / 10.0
    return if shape_a < 0.0 || shape_a > 30.0
    @shape_a = shape_a
    set_arrow_shape(item.canvas)
  end

  def shape_b_c_event(item, event)
    return if event.event_type != Gdk::Event::MOTION_NOTIFY
    return if event.state & Gdk::Window::BUTTON1_MASK == 0
    change = false
    shape_b = (RIGHT - event.x) / 10.0
    if shape_b >= 0.0 && shape_b <= 30.0
      @shape_b = shape_b
      change = true
    end
    shape_c = (MIDDLE - 5 * @width - event.y) / 10.0
    if shape_c >= 0.0
      @shape_c = shape_c
      change = true
    end
    set_arrow_shape(item.canvas) if change
  end

  def create_dimension(root, anchor)
    arrow = Gnome::CanvasLine.new(root,
                                  {:fill_color => "black",
                                    :first_arrowhead => true,
                                    :last_arrowhead => true,
                                    :arrow_shape_a => 5.0,
                                    :arrow_shape_b => 5.0,
                                    :arrow_shape_c => 3.0})
    text = Gnome::CanvasText.new(root,
                                 {:fill_color => "black",
                                   :font => "Sans 12",
                                   :anchor => anchor})
    return arrow, text
  end

  def create_info(root, x, y)
    Gnome::CanvasText.new(root,
                          {:x => x,
                            :y => y,
                            :fill_color => "black",
                            :font => "Sans 14",
                            :anchor => Gtk::ANCHOR_NW})
  end

  def create_sample_arrow(root, x1, y1, x2, y2)
    item = Gnome::CanvasLine.new(root,
                                 {:points => [[x1, y1], [x2, y2]],
                                   :fill_color => "black",
                                   :first_arrowhead => true,
                                   :last_arrowhead => true})
    item
  end

  def initialize
    super(false, 4)
    border_width = 4
    show

    w = Gtk::Label.new(<<EOS)
This demo allows you to edit arrowhead shapes.  Drag the little boxes
to change the shape of the line and its arrowhead.  You can see the
arrows at their normal scale on the right hand side of the window.
EOS
    pack_start(w, false, false, 0)
    w.show()

    w = Gtk::Alignment.new(0.5, 0.5, 0.0, 0.0)
    pack_start(w, true, true, 0)
    w.show()

    frame = Gtk::Frame.new()
    frame.set_shadow_type(Gtk::SHADOW_IN)
    w.add(frame)
    frame.show()

    canvas = Gnome::Canvas.new()
    canvas.set_size_request(500, 350)
    canvas.set_scroll_region(0, 0, 500, 350)
    frame.add(canvas)
    canvas.show()

    root = canvas.root

    @width = DEFAULT_WIDTH
    @shape_a = DEFAULT_SHAPE_A
    @shape_b = DEFAULT_SHAPE_B
    @shape_c = DEFAULT_SHAPE_C

    # Big allow
    @big_arrow = Gnome::CanvasLine.new(root,
                                       {:points => [[LEFT, MIDDLE], [RIGHT, MIDDLE]],
                                         :fill_color => "mediumseagreen",
                                         :width_pixels => DEFAULT_WIDTH * 20,
                                         :last_arrowhead => true})

    # Arrow outline
    @outline = Gnome::CanvasLine.new(root,
                                     {:fill_color => "black",
                                       :width_pixels => 2,
                                       :cap_style => Gdk::GC::CAP_ROUND,
                                       :join_style => Gdk::GC::JOIN_ROUND})

    # Drag boxes
    @width_drag_box = create_drag_box(root) do |item, event|
      width_event(item, event)
    end
    @shape_a_drag_box = create_drag_box(root) do |item, event|
      shape_a_event(item, event)
    end
    @shape_b_c_drag_box = create_drag_box(root) do |item, event|
      shape_b_c_event(item, event)
    end

    # Dimensions
    @width_arrow, @width_text = create_dimension(root, Gtk::ANCHOR_E)
    @shape_a_arrow, @shape_a_text = create_dimension(root, Gtk::ANCHOR_N)
    @shape_b_arrow, @shape_b_text = create_dimension(root, Gtk::ANCHOR_N)
    @shape_c_arrow, @shape_c_text = create_dimension(root, Gtk::ANCHOR_N)

    # Info
    @width_info = create_info(root, LEFT, 260)
    @shape_a_info = create_info(root, LEFT, 280)
    @shape_b_info = create_info(root, LEFT, 300)
    @shape_c_info = create_info(root, LEFT, 320)

    # Division line
    Gnome::CanvasLine.new(root,
                          {:points => [[RIGHT + 50, 0], [RIGHT + 50, 1000]],
                            :fill_color => "black",
                            :width_pixels => 2})

    # Sample arrows
    @sample_1 = create_sample_arrow(root, RIGHT + 100, 30, RIGHT + 100, MIDDLE - 30)
    @sample_2 = create_sample_arrow(root, RIGHT + 70, MIDDLE, RIGHT + 130, MIDDLE)
    @sample_3 = create_sample_arrow(root, RIGHT + 70, MIDDLE + 30, RIGHT + 130, MIDDLE + 120)

    # Done!
    set_arrow_shape(canvas)
  end
end
