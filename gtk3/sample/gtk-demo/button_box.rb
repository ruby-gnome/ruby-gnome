# Copyright (c) 2015 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
=  Button Boxes

 The Button Box widgets are used to arrange buttons with padding.
=end
module ButtonBoxDemo
  def self.run_demo(main_window)
    window = Gtk::Window.new(:toplevel)
    window.screen = main_window.screen
    window.set_title("Button Boxes")
    window.set_border_width(10)

    main_vbox = Gtk::Box.new(:vertical, 0)
    window.add(main_vbox)

    frame_horz = Gtk::Frame.new("Horizontal Button Boxes")
    main_vbox.pack_start(frame_horz, :expand => true, :fill => true, :padding => 10)

    vbox = Gtk::Box.new(:vertical, 0)
    vbox.set_border_width(10)
    frame_horz.add(vbox)

    bbox = create_bbox(true, "Spread", 40, :spread)
    vbox.pack_start(bbox, :expand => true, :fill => true, :padding => 0)

    bbox = create_bbox(true, "Edge", 40, :edge)
    vbox.pack_start(bbox, :expand => true, :fill => true, :padding => 5)

    bbox = create_bbox(true, "Start", 40, :start)
    vbox.pack_start(bbox, :expand => true, :fill => true, :padding => 5)

    bbox = create_bbox(true, "End", 40, :end)
    vbox.pack_start(bbox, :expand => true, :fill => true, :padding => 5)

    bbox = create_bbox(true, "Center", 40, :center)
    vbox.pack_start(bbox, :expand => true, :fill => true, :padding => 5)

    bbox = create_bbox(true, "Expand", 0, :expand)
    vbox.pack_start(bbox, :expand => true, :fill => true, :padding => 5)

    frame_vert = Gtk::Frame.new("Vertical Button Boxes")
    main_vbox.pack_start(frame_vert, :expand => true, :fill => true, :padding => 10)

    hbox = Gtk::Box.new(:horizontal, 0)
    hbox.set_border_width(10)

    frame_vert.add(hbox)

    bbox = create_bbox(false, "Spread", 10, :spread)
    hbox.pack_start(bbox, :expand => true, :fill => true, :padding => 0)

    bbox = create_bbox(false, "Edge", 10, :edge)
    hbox.pack_start(bbox, :expand => true, :fill => true, :padding => 5)

    bbox = create_bbox(false, "Start", 10, :start)
    hbox.pack_start(bbox, :expand => true, :fill => true, :padding => 5)

    bbox = create_bbox(false, "End", 10, :end)
    hbox.pack_start(bbox, :expand => true, :fill => true, :padding => 5)

    bbox = create_bbox(false, "Center", 10, :center)
    hbox.pack_start(bbox, :expand => true, :fill => true, :padding => 5)

    bbox = create_bbox(false, "Expand", 0, :expand)
    hbox.pack_start(bbox, :expand => true, :fill => true, :padding => 5)

    if !window.visible?
      window.show_all
    else
      window.destroy
    end
    window
  end

  def self.create_bbox(horizontal, title, spacing, layout)
    frame = Gtk::Frame.new(title)
    bbox = nil

    if horizontal
      bbox = Gtk::ButtonBox.new(:horizontal)
    else
      bbox = Gtk::ButtonBox.new(:vertical)
    end

    bbox.set_border_width(5)
    frame.add(bbox)
    bbox.set_layout(layout)
    bbox.set_spacing(spacing)

    %w(OK(_O) Cancel(_C) Help(_H)).each do |name|
      button = Gtk::Button.new(:label => name, :use_underline => true)
      bbox.add(button)
    end

    frame
  end
end
