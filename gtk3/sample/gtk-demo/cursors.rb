# Copyright (c) 2015 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
=  Cursors

 Demonstrates a useful set of available cursors.
=end
module CursorsDemo
  def self.run_demo(main_window)
    window = Gtk::Window.new(:toplevel)
    window.screen = main_window.screen
    window.set_title("Cursors")
    window.set_default_size(500, 500)

    sw = Gtk::ScrolledWindow.new(nil, nil)
    sw.set_policy(:never, :automatic)
    window.add(sw)

    box = Gtk::Box.new(:vertical, 0)
    box.set_property("margin-start", 20)
    box.set_property("margin-end", 20)
    box.set_property("margin-bottom", 20)
    sw.add(box)

    section = Section.new(box, "General")
    %w(default none).each do |cursor_name|
      section.add_button(cursor_name)
    end

    section = Section.new(box, "Link & Status")
    %w(context-menu help
       pointer progress wait).each do |cursor_name|
      section.add_button(cursor_name)
    end

    section = Section.new(box, "Selection")
    %w(cell crosshair text vertical-text).each do |cursor_name|
      section.add_button(cursor_name)
    end

    section = Section.new(box, "Drag & Drop")
    %w(alias copy move no-drop
       not-allowed grab grabbing).each do |cursor_name|
      section.add_button(cursor_name)
    end

    section = Section.new(box, "Resize & Scrolling")
    %w(all-scroll col-resize row-resize n-resize
       e-resize s-resize w-resize ne-resize nw-resize
       se-resize sw-resize ew-resize ns-resize nesw-resize
       nwse-resize).each do |cursor_name|
      section.add_button(cursor_name)
    end

    section = Section.new(box, "Zoom")
    %w(zoom-in zoom-out).each do |cursor_name|
      section.add_button(cursor_name)
    end

    if !window.visible?
      window.show_all
    else
      window.destroy
    end
    window
  end

  class Section
    def initialize(container, title)
      label = Gtk::Label.new(title)
      label.set_xalign(0.0)
      label.set_margin_top(10)
      label.set_margin_bottom(10)
      container.pack_start(label, :expand => false, :fill => true, :padding => 0)

      @section = Gtk::FlowBox.new
      @section.set_halign(:start)
      @section.set_selection_mode(:none)
      @section.set_min_children_per_line(2)
      @section.set_min_children_per_line(20)
      container.pack_start(@section, :expand => false, :fill => true, :padding => 0)
    end

    def add_button(css_name)
      cursor = Gdk::Cursor.new(css_name)
      image = nil
      if !cursor
        image = Gtk::Image.new(:icon_name => "image-missing", :size => :menu)
      else
        path = "/cursors/#{css_name.tr('-', '_')}_cursor.png"
        image = Gtk::Image.new(:resource => path)
      end
      image.set_size_request(32, 32)

      button = Gtk::Button.new
      button.add(image)
      button.style_context.add_class("image-button")
      button.signal_connect("clicked") do |_widget|
        apply_cursor(cursor)
      end
      button.set_tooltip_text(css_name)
      @section.add(button)
    end

    private

    def apply_cursor(cursor)
      toplevel = @section.toplevel
      window = toplevel.window
      window.set_cursor(cursor)
    end
  end
end
