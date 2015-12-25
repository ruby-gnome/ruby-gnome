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
    window.set_border_width(8)

    vbox = Gtk::Box.new(:vertical, 0)
    window.add(vbox)

    generate_simple_infobar_and_button("info", vbox, actions)
    generate_simple_infobar_and_button("warning", vbox, actions)

    bar = Gtk::InfoBar.new
    bar.add_button("_OK", Gtk::ResponseType::OK)
    bar.set_show_close_button(true)
    bar.signal_connect "response" do |info_bar, response_id|
      info_bar.hide if response_id == Gtk::ResponseType::CLOSE
      dialog = Gtk::MessageDialog.new(:parent => info_bar.toplevel,
                                      :flags => [:modal, :destroy_with_parent],
                                      :type => :info,
                                      :buttons => :ok,
                                      :message => "You clicked a button on an info bar")
      dialog.secondary_text = "Your response has id #{response_id}"
      dialog.signal_connect("response") { |widget| widget.destroy }
      dialog.show_all
    end

    vbox.pack_start(bar,
                    :expand => false,
                    :fill => false,
                    :padding => 0)

    bar.set_message_type(:question)
    label = Gtk::Label.new("This is an info bar with message type Gtk::MessageType::QUESTION")
    label.set_line_wrap(true)
    label.set_xalign(0)

    bar.content_area.pack_start(label, :expand => false, :fill => false, :padding => 0)

    button = Gtk::ToggleButton.new(:label => "Question")
    button.bind_property("active", bar, "visible", :bidirectional)
    actions.add(button)
    button.set_active(false)

    generate_simple_infobar_and_button("error", vbox, actions)
    generate_simple_infobar_and_button("other", vbox, actions)

    frame = Gtk::Frame.new("Info bars")
    vbox.pack_start(frame, :expand => false, :fill => false, :padding => 8)

    vbox2 = Gtk::Box.new(:vertical, 8)
    vbox2.set_border_width(8)
    frame.add(vbox2)

    label = Gtk::Label.new("An example of different info bars")
    vbox2.pack_start(label, :expand => false, :fill => false, :padding => 0)
    actions.show_all
    vbox2.pack_start(actions, :expand => false, :fill => false, :padding => 0)

    if !window.visible?
      window.show_all
    else
      window.destroy
    end
    window
  end

  def self.generate_simple_infobar_and_button(message_type, bar_parent, button_parent)
    bar = Gtk::InfoBar.new
    bar_parent.pack_start(bar, :expand => false, :fill => false, :padding => 0)
    bar.set_message_type(message_type.to_sym)

    label = Gtk::Label.new("This is an info bar with message type Gtk::MessageType::#{message_type.upcase}")
    label.set_line_wrap(true)
    label.set_xalign(0)

    bar.content_area.pack_start(label, :expand => false, :fill => false, :padding => 0)

    button = Gtk::ToggleButton.new(:label => "Message")
    button.bind_property("active", bar, "visible", :bidirectional)
    button_parent.add(button)
  end
end
