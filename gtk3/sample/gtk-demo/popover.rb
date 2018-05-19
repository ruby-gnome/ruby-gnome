# Copyright (c) 2015-2016 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
=  Popovers

 A bubble-like window containing contextual information or options.
 GtkPopovers can be attached to any widget, and will be displayed
 within the same window, but on top of all its content.
=end
class PopoverDemo
  def initialize(main_window)
    @window = Gtk::Window.new(:toplevel)
    @window.screen = main_window.screen
    box = Gtk::Box.new(:vertical, 24)
    box.margin = 24
    @window.add(box)

    widget = add_toggle_button_with_popover
    box.add(widget)

    widget = add_custom_entry_with_complex_popover
    box.add(widget)

    widget = add_calendar_with_popover
    box.add(widget)
  end

  def run
    if !@window.visible?
      @window.show_all
    else
      @window.destroy
    end
    @window
  end

  def create_popover(parent, child, pos)
    popover = Gtk::Popover.new(parent)
    popover.position = pos
    popover.add(child)
    child.margin = 6
    child.show
    popover
  end

  def create_complex_popover(parent, pos)
    if Gtk::Version.or_later?(3, 20)
      builder = Gtk::Builder.new(:resource => "/popover/popover.ui")
    else
      builder = Gtk::Builder.new(:resource => "/popover/popover-3.18.ui")
    end
    window = builder["window"]
    content = window.child
    content.parent.remove(content)
    window.destroy
    popover = create_popover(parent, content, pos)
    popover
  end

  def add_toggle_button_with_popover
    widget = Gtk::ToggleButton.new(:label => "Button")
    label = Gtk::Label.new("This popover does not grab input")
    toggle_popover = create_popover(widget, label, :top)
    toggle_popover.modal = false
    widget.signal_connect "toggled" do |button|
      toggle_popover.visible = button.active?
    end
    widget
  end

  def add_custom_entry_with_complex_popover
    widget = CustomEntry.new
    entry_popover = create_complex_popover(widget, :top)
    widget.set_icon_from_icon_name(:primary, "edit-find")
    widget.set_icon_from_icon_name(:secondary, "edit-clear")
    widget.signal_connect "icon-press" do |entry, icon_pos, _event|
      rect = entry.get_icon_area(icon_pos)
      entry_popover.pointing_to = rect
      entry_popover.show
      entry.popover_icon_pos = icon_pos
    end

    widget.signal_connect "size-allocate" do |entry, _allocation|
      if entry_popover.visible?
        popover_pos = entry.popover_icon_pos
        rect = entry.get_icon_area(popover_pos)
        entry_popover.pointing_to = rect
      end
    end
    widget
  end

  def add_calendar_with_popover
    widget = Gtk::Calendar.new
    widget.signal_connect "day-selected" do |calendar|
      event = Gtk.current_event
      if event.type == :button_press
        x, y = event.window.coords_to_parent(event.x, event.y)
        allocation = calendar.allocation
        rect = Gdk::Rectangle.new(x - allocation.x, y - allocation.y, 1, 1)
        cal_popover = create_popover(calendar, CustomEntry.new, :bottom)
        cal_popover.pointing_to = rect
        cal_popover.show
      end
    end
    widget
  end
end

class CustomEntry < Gtk::Entry
  attr_accessor :popover_icon_pos
  def initialize
    super
  end
end
