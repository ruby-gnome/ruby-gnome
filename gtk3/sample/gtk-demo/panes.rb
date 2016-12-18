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
class PanesDemo
  def initialize(main_window)
    @window = Gtk::Window.new(:toplevel)
    @window.screen = main_window.screen
    @window.title = "Paned Widgets"

    vbox = Gtk::Box.new(:vertical, 0)
    @window.add(vbox)

    vpaned = Gtk::Paned.new(:vertical)
    vbox.pack_start(vpaned, :expand => true, :fill => true, :padding => 0)
    vpaned.margin = 5

    @hpaned = Gtk::Paned.new(:horizontal)
    vpaned.add1(@hpaned)

    frame = Gtk::Frame.new
    frame.shadow_type = :in
    frame.set_size_request(60, 60)
    @hpaned.add1(frame)

    button = Gtk::Button.new(:label => "_Hi there", :use_underline => true)
    frame.add(button)

    frame = Gtk::Frame.new
    frame.shadow_type = :in
    frame.set_size_request(80, 60)
    @hpaned.add2(frame)

    frame = Gtk::Frame.new
    frame.shadow_type = :in
    frame.set_size_request(60, 80)
    vpaned.add2(frame)

    # Now create toggle buttons to control sizing
    buttons = create_pane_options("Horizontal",
                                  "Left",
                                  "Right")
    vbox.pack_start(buttons, :expand => false, :fill => false, :padding => 0)
    buttons = create_pane_options("Vertical",
                                  "Top",
                                  "Bottom")
    vbox.pack_start(buttons, :expand => false, :fill => false, :padding => 0)
    vbox.show_all
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

  def create_pane_options(frame_label, label1, label2)
    child1 = @hpaned.child1
    child2 = @hpaned.child2
    frame = Gtk::Frame.new(frame_label)
    frame.margin = 4

    @table = Gtk::Grid.new
    frame.add(@table)

    label = Gtk::Label.new(label1)
    @table.attach(label, 0, 0, 1, 1)

    check_button(:resize, false, child1, 0, 1)
    check_button(:shrink, true, child1, 0, 2)

    label = Gtk::Label.new(label2)
    @table.attach(label, 1, 0, 1, 1)

    check_button(:resize, true, child2, 1, 1)
    check_button(:shrink, true, child2, 1, 2)
    frame
  end

  def check_button(type, active, child, xposition, yposition)
    is_resize = (type == :resize)
    label = is_resize ? "_Resize" : "_Shrink"
    check_button = Gtk::CheckButton.new(label)
    check_button.use_underline = true
    check_button.active = active
    @table.attach(check_button, xposition, yposition, 1, 1)
    check_button.signal_connect "toggled" do
      is_child1 = (@hpaned.child1 == child)

      resize = @hpaned.child_get_property(child, "resize")
      shrink = @hpaned.child_get_property(child, "shrink")

      if is_resize
        resize = !resize
      else
        shrink = !shrink
      end

      @hpaned.remove(child)
      if is_child1
        @hpaned.pack1(child, :resize => resize, :shrink => shrink)
      else
        @hpaned.pack2(child, :resize => resize, :shrink => shrink)
      end
    end
  end
end
