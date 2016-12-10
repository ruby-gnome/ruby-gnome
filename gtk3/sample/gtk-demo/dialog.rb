# Copyright (c) 2015-2016 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
=  Dialogs and Message Boxes

 Dialog widgets are used to pop up a transient window for user feedback.
=end
class DialogDemo
  def initialize(main_window)
    @window = Gtk::Window.new(:toplevel)
    @window.screen = main_window.screen
    @window.title = "Dialogs and Message Boxes"

    frame = Gtk::Frame.new("Dialogs")
    frame.margin = 8
    @window.add(frame)

    vbox = Gtk::Box.new(:vertical, 8)
    vbox.margin = 8
    frame.add(vbox)

    # Standard message dialog
    hbox = Gtk::Box.new(:horizontal, 8)
    vbox.pack_start(hbox, :expand => false, :fill => false, :padding => 0)

    button = initialize_standard_message_button
    hbox.pack_start(button, :expand => false, :fill => false, :padding => 0)
    vbox.pack_start(Gtk::Separator.new(:horizontal),
                    :expand => false,
                    :fill => false,
                    :padding => 0)

    # Interactive dialog
    hbox = Gtk::Box.new(:horizontal, 8)
    vbox.pack_start(hbox, :expand => false, :fill => false, :padding => 0)
    vbox2 = Gtk::Box.new(:vertical, 0)

    hbox.pack_start(vbox2, :expand => false, :fill => false, :padding => 0)
    button = initialize_interactive_message_button
    vbox2.pack_start(button, :expand => false, :fill => false, :padding => 0)

    table, @entry1, @entry2 = initialize_grid_with_entries
    hbox.pack_start(table, :expand => false, :fill => false, :padding => 0)
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

  def initialize_standard_message_button
    button = Gtk::Button.new(:label => "_Message Dialog",
                             :use_underline => true)
    @counter = 0
    button.signal_connect "clicked" do
      show_standard_message_dialog
    end
    button
  end

  def initialize_interactive_message_button
    button = Gtk::Button.new(:label => "_Interactive Dialog",
                             :use_underline => true)
    button.signal_connect "clicked" do
      show_interactive_message_dialog
    end
    button
  end

  def show_standard_message_dialog
    dialog = Gtk::MessageDialog.new(:parent => @window,
                                    :flags => [:modal, :destroy_with_parent],
                                    :type => :info,
                                    :buttons => :ok_cancel,
                                    :message => <<-MESSAGE)
This message has been popped up the following
number of times:
MESSAGE
    dialog.secondary_text = @counter.to_s
    dialog.run
    dialog.destroy
    @counter += 1
  end

  def show_interactive_message_dialog
    dialog = Gtk::Dialog.new(:parent => @window,
                             :title => "Interactive Dialog",
                             :flags => [:modal, :destroy_with_parent],
                             :buttons => [["_OK", :ok],
                                          ["_Cancel", :cancel]])
    content_area = dialog.content_area
    hbox = initialize_interactive_dialog_interface
    content_area.pack_start(hbox)

    response = dialog.run

    if response == :ok
      @entry1.text = @dialog_entry1.text
      @entry2.text = @dialog_entry2.text
    end

    dialog.destroy
  end

  def initialize_interactive_dialog_interface
    hbox = Gtk::Box.new(:horizontal, 8)
    image = Gtk::Image.new(:icon_name => "dialog-question", :size => :dialog)
    hbox.pack_start(image)
    table, @dialog_entry1, @dialog_entry2 = initialize_grid_with_entries
    hbox.pack_start(table, :expand => false, :fill => false, :padding => 0)
    hbox.show_all
    hbox
  end

  def initialize_grid_with_entries
    table = Gtk::Grid.new
    table.row_spacing = 4
    table.column_spacing = 4

    label = Gtk::Label.new("_Entry 1", :use_underline => true)
    table.attach(label, 0, 0, 1, 1)

    entry1 = Gtk::Entry.new
    table.attach(entry1, 1, 0, 1, 1)
    label.set_mnemonic_widget(entry1)

    label = Gtk::Label.new("E_ntry 2", :use_underline => true)
    table.attach(label, 0, 1, 1, 1)

    entry2 = Gtk::Entry.new
    table.attach(entry2, 1, 1, 1, 1)
    label.set_mnemonic_widget(entry2)

    [table, entry1, entry2]
  end
end
