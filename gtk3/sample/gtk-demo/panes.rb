# Copyright (c) 2016 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
=  Paned Widgets

 The GtkPaned Widget divides its content area into two panes
 with a divider in between that the user can adjust. A separate
 child is placed into each pane. GtkPaned widgets can be split
 horizontally or vertically.

 There are a number of options that can be set for each pane.
 This test contains both a horizontal and a vertical GtkPaned
 widget, and allows you to adjust the options for each side of
 each widget.
=end
module PanesDemo
  def self.run_demo(main_window)
    window = Gtk::Window.new(:toplevel)
    window.screen = main_window.screen
    window.title = "Paned Widgets"
    window.border_width = 0

    vbox = Gtk::Box.new(:vertical, 0)
    window.add(vbox)

    vpaned = Gtk::Paned.new(:vertical)
    vbox.pack_start(vpaned, :expand => true, :fill => true, :padding => 0)
    vpaned.border_width = 5

    hpaned = Gtk::Paned.new(:horizontal)
    vpaned.add1(hpaned)

    frame = Gtk::Frame.new
    frame.shadow_type = :in
    frame.set_size_request(60, 60)
    hpaned.add1(frame)

    button = Gtk::Button.new(:label => "_Hi there", :use_underline => true)
    frame.add(button)

    frame = Gtk::Frame.new
    frame.shadow_type = :in
    frame.set_size_request(80, 60)
    hpaned.add2(frame)

    frame = Gtk::Frame.new
    frame.shadow_type = :in
    frame.set_size_request(60, 80)
    vpaned.add2(frame)

    # Now create toggle buttons to control sizing
    buttons = create_pane_options(hpaned,
                                  "Horizontal",
                                  "Left",
                                  "Right")
    vbox.pack_start(buttons, :expand => false, :fill => false, :padding => 0)
    buttons = create_pane_options(hpaned,
                                  "Vertical",
                                  "Top",
                                  "Bottom")
    vbox.pack_start(buttons, :expand => false, :fill => false, :padding => 0)
    vbox.show_all

    if !window.visible?
      window.show_all
    else
      window.destroy
    end
    window
  end

  def self.create_pane_options(paned, frame_label, label1, label2)
    child1 = paned.child1
    child2 = paned.child2

    frame = Gtk::Frame.new(frame_label)
    frame.border_width = 4

    table = Gtk::Grid.new
    frame.add(table)

    label = Gtk::Label.new(label1)
    table.attach(label, 0, 0, 1, 1)

    check_button = Gtk::CheckButton.new("_Resize")
    check_button.use_underline = true
    table.attach(check_button, 0, 1, 1, 1)
    check_button.signal_connect "toggled" do
      resize = paned.child_get_property(child1, "resize")
      shrink = paned.child_get_property(child1, "shrink")

      paned.remove(child1)
      paned.pack1(child1, :resize => !resize, :shrink => shrink)
    end

    check_button = Gtk::CheckButton.new("_Shrink")
    check_button.use_underline = true
    table.attach(check_button, 0, 2, 1, 1)
    check_button.active = true
    check_button.signal_connect "toggled" do
      resize = paned.child_get_property(child1, "resize")
      shrink = paned.child_get_property(child1, "shrink")

      paned.remove(child1)
      paned.pack1(child1, :resize => resize, :shrink => !shrink)
    end

    label = Gtk::Label.new(label2)
    table.attach(label, 1, 0, 1, 1)

    check_button = Gtk::CheckButton.new("_Resize")
    check_button.use_underline = true
    table.attach(check_button, 1, 1, 1, 1)
    check_button.active = true
    check_button.signal_connect "toggled" do
      resize = paned.child_get_property(child2, "resize")
      shrink = paned.child_get_property(child2, "shrink")

      paned.remove(child2)
      paned.pack2(child2, :resize => !resize, :shrink => shrink)
    end

    check_button = Gtk::CheckButton.new("_Shrink")
    check_button.use_underline = true
    table.attach(check_button, 1, 2, 1, 1)
    check_button.active = true
    check_button.signal_connect "toggled" do
      resize = paned.child_get_property(child2, "resize")
      shrink = paned.child_get_property(child2, "shrink")

      paned.remove(child2)
      paned.pack2(child2, :resize => resize, :shrink => !shrink)
    end
    frame
  end
end
