#  -*- indent-tabs-mode: nil -*-
class CanvasSampleFeatures < Gtk::VBox

  PAD_SMALL = 4

  def item_event(item, event)
    if (event.event_type != Gdk::BUTTON_PRESS) || (event.button != 1)
      return
    end
    if (item.parent == @parent1)
      item.reparent(@parent2)
    else
      item.reparent(@parent1)
    end
  end

  def initialize

    super(false, PAD_SMALL)
    border_width = PAD_SMALL
    show()

    # Instructions
    w = Gtk::Label.new("Reparent test:  click on the items to switch them between parents")
    pack_start(w, false, false, 0)
    w.show()

    # Frame and canvas
    alignment = Gtk::Alignment.new(0.5, 0.5, 0.0, 0.0)
    pack_start(alignment, false, false, 0)
    alignment.show()

    frame = Gtk::Frame.new()
    frame.set_shadow_type(Gtk::SHADOW_IN);
    alignment.add(frame)
    frame.show()

    canvas = Gnome::Canvas.new()
    canvas.set_usize(400, 200)
    canvas.set_scroll_region(0, 0, 400, 200)
    frame.add(canvas)
    canvas.show()

    # First parent and box
    @parent1 = Gnome::CanvasGroup.new(canvas.root,
                                      {"x" => 0.0,
                                        "y" => 0.0})

    Gnome::CanvasRect.new(@parent1,
                          {"x1" => 0.0,
                            "y1" => 0.0,
                            "x2" => 200.0,
                            "y2" => 200.0,
                            "fill_color" => "tan"})

    # Second parent and box
    @parent2 = Gnome::CanvasGroup.new(canvas.root,
                                      {"x" => 200.0,
                                        "y" => 0.0})
    Gnome::CanvasRect.new(@parent2,
                          {"x1" => 0.0,
                            "y1" => 0.0,
                            "x2" => 200.0,
                            "y2" => 200.0,
                            "fill_color" => "#204060"})
    
    # Big circle to be reparented
    item = Gnome::CanvasEllipse.new(@parent1,
                                    {"x1" => 10.0,
                                      "y1" => 10.0,
                                      "x2" => 190.0,
                                      "y2" => 190.0,
                                      "outline_color" => "black",
                                      "fill_color" => "mediumseagreen",
                                      "width_units" => 3.0})

    item.signal_connect("event") do |item, event|
      item_event(item, event)
    end

    # A group to be reparented
    group = Gnome::CanvasGroup.new(@parent2,
                                   {"x" => 100.0,
                                     "y" => 100.0})
    Gnome::CanvasEllipse.new(group,
                             {"x1" => -50.0,
                               "y1" => -50.0,
                               "x2" => 50.0,
                               "y2" => 50.0,
                               "outline_color" => "black",
                               "fill_color" => "wheat",
                               "width_units" => 3.0})
    Gnome::CanvasEllipse.new(group,
                             {"x1" => -25.0,
                               "y1" => -25.0,
                               "x2" => 25.0,
                               "y2" => 25.0,
                               "fill_color" => "steelblue"})

    group.signal_connect("event") do |item, event|
      item_event(item, event)
    end

    # Done
  end
end
