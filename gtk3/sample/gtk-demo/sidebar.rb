# Copyright (c) 2015-2016 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
= Stack Sidebar

GtkStackSidebar provides an automatic sidebar widget to control
navigation of a GtkStack object. This widget automatically updates it
content based on what is presently available in the GtkStack object,
and using the "title" child property to set the display labels.
=end
class SidebarDemo
  def initialize(_main_window)
    @window = Gtk::Window.new(:toplevel)
    @window.resizable = true
    @window.set_size_request(500, 350)

    header = Gtk::HeaderBar.new
    header.show_close_button = true
    @window.titlebar = header
    @window.title = "Stack Sidebar"

    box = Gtk::Box.new(:horizontal, 0)
    sidebar = Gtk::StackSidebar.new
    box.pack_start(sidebar, :expand => false, :fill => false, :padding => 0)

    stack = Gtk::Stack.new
    stack.transition_type = :slide_up_down
    sidebar.stack = stack

    widget = Gtk::Separator.new(:vertical)
    box.pack_start(widget, :expand => false, :fill => false, :padding => 0)

    box.pack_start(stack, :expand => true, :fill => true, :padding => 0)

    pages = ["Welcome to GTK+",
             "GtkStackSidebar Widget",
             "Automatic navigation",
             "Consistent appearance",
             "Scrolling",
             "Page 6",
             "Page 7",
             "Page 8",
             "Page 9"]

    pages.each_with_index do |page_string, i|
      child = nil
      if i.zero?
        child = Gtk::Image.new(:icon_name => "help-about", :size => :menu)
        child.pixel_size = 256
      else
        child = Gtk::Label.new(page_string)
      end

      stack.add_named(child, page_string)
      stack.child_set_property(child, "title", page_string)
    end

    @window.add(box)
  end

  def run
    if !@window.visible?
      @window.show_all
    else
      @window.destroy
    end
  end
end
