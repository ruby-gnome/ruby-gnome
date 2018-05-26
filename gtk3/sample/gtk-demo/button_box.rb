# Copyright (c) 2015-2016 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
=  Button Boxes

 The Button Box widgets are used to arrange buttons with padding.
=end
class ButtonBoxDemo
  def initialize(main_window)
    @window = Gtk::Window.new(:toplevel)
    @window.screen = main_window.screen
    @window.title = "Button Boxes"

    main_vbox = Gtk::Box.new(:vertical, 0)
    @window.add(main_vbox)
    frame_horz = generate_horizontal_frame
    main_vbox.pack_start(frame_horz,
                         :expand => true, :fill => true, :padding => 0)
    frame_vert = generate_vertical_frame
    main_vbox.pack_start(frame_vert,
                         :expand => true, :fill => true, :padding => 0)
  end

  def run
    if !@window.visible?
      @window.show_all
    else
      @window.destroy
    end
    @window
  end

  private

  def generate_horizontal_frame
    frame_horz = Gtk::Frame.new("Horizontal Button Boxes")
    frame_horz.margin_top = 10
    frame_horz.margin_bottom = 10
    vbox = Gtk::Box.new(:vertical, 10)
    frame_horz.add(vbox)

    bbox = create_bbox(true, "Spread", 40, :spread)
    vbox.pack_start(bbox, :expand => true, :fill => true, :padding => 0)

    bbox = create_bbox(true, "Edge", 40, :edge)
    vbox.pack_start(bbox, :expand => true, :fill => true, :padding => 0)

    bbox = create_bbox(true, "Start", 40, :start)
    vbox.pack_start(bbox, :expand => true, :fill => true, :padding => 0)

    bbox = create_bbox(true, "End", 40, :end)
    vbox.pack_start(bbox, :expand => true, :fill => true, :padding => 0)

    bbox = create_bbox(true, "Center", 40, :center)
    vbox.pack_start(bbox, :expand => true, :fill => true, :padding => 0)

    bbox = create_bbox(true, "Expand", 0, :expand)
    vbox.pack_start(bbox, :expand => true, :fill => true, :padding => 0)
    frame_horz
  end

  def generate_vertical_frame
    frame_vert = Gtk::Frame.new("Vertical Button Boxes")

    hbox = Gtk::Box.new(:horizontal, 10)

    frame_vert.add(hbox)

    bbox = create_bbox(false, "Spread", 10, :spread)
    hbox.pack_start(bbox, :expand => true, :fill => true, :padding => 0)

    bbox = create_bbox(false, "Edge", 10, :edge)
    hbox.pack_start(bbox, :expand => true, :fill => true, :padding => 0)

    bbox = create_bbox(false, "Start", 10, :start)
    hbox.pack_start(bbox, :expand => true, :fill => true, :padding => 0)

    bbox = create_bbox(false, "End", 10, :end)
    hbox.pack_start(bbox, :expand => true, :fill => true, :padding => 0)

    bbox = create_bbox(false, "Center", 10, :center)
    hbox.pack_start(bbox, :expand => true, :fill => true, :padding => 0)

    bbox = create_bbox(false, "Expand", 0, :expand)
    hbox.pack_start(bbox, :expand => true, :fill => true, :padding => 0)
    frame_vert
  end

  def create_bbox(horizontal, title, spacing, layout)
    frame = Gtk::Frame.new(title)

    orientation = horizontal ? :horizontal : :vertical
    bbox = Gtk::ButtonBox.new(orientation)

    frame.add(bbox)
    bbox.layout = layout
    bbox.spacing = spacing

    %w(OK(_O) Cancel(_C) Help(_H)).each do |name|
      button = Gtk::Button.new(:label => name, :use_underline => true)
      bbox.add(button)
    end

    frame
  end
end
