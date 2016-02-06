# Copyright (c) 2015 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
=  Popovers  
 
 A bubble-like window containing contextual information or options. 
 GtkPopovers can be attached to any widget, and will be displayed 
 within the same window, but on top of all its content. 
=end
module PopoverDemo
  def self.run_demo(main_window)
    window = Gtk::Window.new(:toplevel)
    window.screen = main_window.screen
    box = Gtk::Box.new(:vertical, 24)
    box.border_width = 24
    window.add(box)

    widget = Gtk::ToggleButton.new(:label => "Button")
    popover = create_popover(widget,
                             Gtk::Label.new("This popover does not grab input"),
                             :top)
    popover.modal = false
    widget.signal_connect "toggled" do |button|
      popover.visible = button.active?
    end

    box.add(widget)

    widget = CustomEntry.new
    popover = create_complex_popover(widget, :top)
    widget.set_icon_from_icon_name(:primary, "edit-find")
    widget.set_icon_from_icon_name(:secondary, "edit-clear")

    widget.signal_connect "icon-press" do  |entry, icon_pos, event|
      rect = entry.get_icon_area(icon_pos)
      popover.set_pointing_to(rect)
      popover.show
      entry.popover_icon_pos = icon_pos
    end

    widget.signal_connect "size-allocate" do |entry, allocation|
      if popover.visible?
        popover_pos = entry.popover_icon_pos
        rect = entry.get_icon_area(popover_pos)
        popover.set_pointing_to(rect)
      end
    end

    box.add(widget)

    widget = Gtk::Calendar.new
    widget.signal_connect "day-selected" do |calendar|
      event = Gtk.current_event
      if event.type == :button_press
        x, y = event.button.window.coords_to_parent(event.button.x,
                                                    event.button.y)
        allocation = calendar.allocation
        rect = Cairo::Rectangle.new(x - allocation.x,
                                    y - allocation.y,
                                    1,
                                    1)
        popover = create_popover(calendar, CustomEntry.new, :bottom)
        popover.set_pointing_to(popover, rect)
        popover.show
      end
    end

    box.add(widget)

    if !window.visible?
      window.show_all
    else
      window.destroy
    end
    window
  end
  
  def self.create_popover(parent, child, pos)
    popover = Gtk::Popover.new(parent)
    popover.position = pos
    popover.add(child)
    popover.border_width = 6
    child.show
    popover
  end

  def self.create_complex_popover(parent, pos)
    builder = Gtk::Builder.new(:resource => "/popover/popover.ui")
    window = builder["window"]
    content = window.child
    content.parent.remove(content)
    window.destroy
    popover = create_popover(parent, content, :bottom)
    popover.set_size_request(200, -1)
    popover.vexpand = true
    popover.margin_start = 10
    popover.margin_end = 10
    popover.margin_bottom = 10
    popover
  end

  class CustomEntry < Gtk::Entry
    attr_accessor :popover_icon_pos
    def initialize
      super
    end
  end
end
