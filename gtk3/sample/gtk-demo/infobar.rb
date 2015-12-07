# Copyright (c) 2015 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
=  Info Bars  
 
 Info bar widgets are used to report important messages to the user. 
=end
module InfobarDemo
  def self.run_demo(main_window)
    actions = Gtk::Box.new(:horizontal, 0)
    
    window = Gtk::Window.new(:toplevel)
    window.screen = main_window.screen
    window.set_title("Info Bars")
    window.set_border_with(8)

    vbox = Gtk::Box.new(:vertical, 0)
    window.add(vbox)

    bar = Gtk::InfoBar.new
    vbox.pack_start(bar,
                    :expand => false,
                    :fill => false,
                    :padding => 0)

    bar.set_message_type(:info)
    
    label = Gtk::Label.new("This is an info bar with message type Gtk::MessageType::INFO")
    label.set_line_wrap(true)
    label.set_xalign(0)
    
    bar.content_area.pack_start(label,
                                :expand => false,
                                :fill => false,
                                :padding => 0)

    button = Gtk::ToggleButton.new(:label => "Message")
    button.bind_property("active", bar, "visible", :bidirectional)
    actions.add(button)

    bar = Gtk::InfoBar.new
    vbox.pack_start(bar,
                    :expand => false,
                    :fill => false,
                    :padding => 0)

    bar.set_message_type(:warning)
    
    label = Gtk::Label.new("This is an info bar with message type Gtk::MessageType::WARNING")
    label.set_line_wrap(true)
    label.set_xalign(0)
    
    bar.content_area.pack_start(label,
                                :expand => false,
                                :fill => false,
                                :padding => 0)

    button = Gtk::ToggleButton.new(:label => "Warning")
    button.bind_property("active", bar, "visible", :bidirectional)
    actions.add(button)

    bar
    if !window.visible?
      window.show_all
    else
      window.destroy
    end
    window
  end
end
