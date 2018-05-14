# Copyright (c) 2015-2016 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
=  Info Bars

 Info bar widgets are used to report important messages to the user.
=end
class InfobarDemo
  def initialize(main_window)
    @actions = Gtk::Box.new(:horizontal, 0)

    @window = Gtk::Window.new(:toplevel)
    @window.screen = main_window.screen
    @window.set_title("Info Bars")
    @window.margin = 8

    @vbox = Gtk::Box.new(:vertical, 0)
    @window.add(@vbox)

    generate_simple_infobar("info")
    generate_simple_infobar("warning")

    generate_infobar_with_dialog

    generate_simple_infobar("error")
    generate_simple_infobar("other")

    frame = Gtk::Frame.new("Info bars")
    @vbox.pack_start(frame, :expand => false, :fill => false, :padding => 0)
    frame.margin_top = 8
    frame.margin_bottom = 8

    vbox2 = Gtk::Box.new(:vertical, 8)
    vbox2.margin = 8
    frame.add(vbox2)

    label = Gtk::Label.new("An example of different info bars")
    vbox2.pack_start(label, :expand => false, :fill => false, :padding => 0)
    @actions.show_all
    vbox2.pack_start(@actions, :expand => false, :fill => false, :padding => 0)
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

  def generate_infobar_label(message)
    label = Gtk::Label.new(message)
    label.line_wrap = true
    label.xalign = 0
    label
  end

  def add_a_label_message_in_an_infobar(bar, message)
    label = generate_infobar_label(message)
    bar.content_area.pack_start(label, :expand => false, :fill => false, :padding => 0)
  end

  def generate_simple_infobar(message_type)
    bar = Gtk::InfoBar.new
    @vbox.pack_start(bar, :expand => false, :fill => false, :padding => 0)
    bar.message_type = message_type.to_sym

    message = "This is an info bar with message type Gtk::MessageType::#{message_type.upcase}"
    add_a_label_message_in_an_infobar(bar, message)

    link_bar_to_a_toggle_button(bar, message_type.capitalize)
  end

  def link_bar_to_a_toggle_button(bar, message)
    button = Gtk::ToggleButton.new(:label => message)
    button.bind_property("active", bar, "visible", :bidirectional)
    @actions.add(button)
    button
  end

  def generate_infobar_with_dialog
    bar = Gtk::InfoBar.new
    bar.add_button("_OK", Gtk::ResponseType::OK)
    bar.show_close_button = true

    manage_response_event_of_infobar(bar)

    @vbox.pack_start(bar, :expand => false, :fill => false, :padding => 0)

    bar.message_type = :question
    message = "This is an info bar with message type Gtk::MessageType::QUESTION"
    add_a_label_message_in_an_infobar(bar, message)

    button = link_bar_to_a_toggle_button(bar, "Question")
    button.active = false
  end

  def manage_response_event_of_infobar(bar)
    bar.signal_connect "response" do |info_bar, response_id|
      info_bar.hide if response_id == Gtk::ResponseType::CLOSE
      dialog = Gtk::MessageDialog.new(:parent => info_bar.toplevel,
                                      :flags => [:modal, :destroy_with_parent],
                                      :type => :info,
                                      :buttons => :ok,
                                      :message => "You clicked a button on an info bar")
      dialog.secondary_text = "Your response has id #{response_id}"
      dialog.signal_connect("response") do
        dialog.destroy
      end
      dialog.show_all
    end
  end
end
