# Copyright (c) 2015 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
=  Dialogs and Message Boxes

 Dialog widgets are used to pop up a transient window for user feedback.
=end
module DialogDemo
  def self.run_demo(main_window)
    window = Gtk::Window.new(:toplevel)
    window.screen = main_window.screen
    window.title = "Dialogs and Message Boxes"
    window.border_width = 8

    frame = Gtk::Frame.new("Dialogs")
    window.add(frame)

    vbox = Gtk::Box.new(:vertical, 8)
    vbox.border_width = 8
    frame.add(vbox)

    # Standard message dialog
    hbox = Gtk::Box.new(:horizontal, 8)
    vbox.pack_start(hbox, :expand => false, :fill => false, :padding => 0)
    button = Gtk::Button.new(:label => "_Message Dialog",
                             :use_underline => true)
    i = 0

    button.signal_connect "clicked" do
      dialog = Gtk::MessageDialog.new(:parent => window,
                                      :flags => [:modal, :destroy_with_parent],
                                      :type => :info,
                                      :buttons => :ok_cancel,
                                      :message => <<-MESSAGE)
This message has been popped up the following
number of times:
MESSAGE
      dialog.secondary_text = "#{i}"
      dialog.run
      dialog.destroy
      i += 1
    end

    hbox.pack_start(button, :expand => false, :fill => false, :padding => 0)
    vbox.pack_start(Gtk::Separator.new(:horizontal),
                    :expand => false,
                    :fill => false,
                    :padding => 0)

    # Interactive dialog
    hbox = Gtk::Box.new(:horizontal, 8)
    vbox.pack_start(hbox, :expand => false, :fill => false, :padding => 0)
    vbox2 = Gtk::Box.new(:vertical, 0)

    button = Gtk::Button.new(:label => "_Interactive Dialog",
                             :use_underline => true)

    hbox.pack_start(vbox2, :expand => false, :fill => false, :padding => 0)
    vbox2.pack_start(button, :expand => false, :fill => false, :padding => 0)

    table = Gtk::Grid.new
    table.row_spacing = 4
    table.column_spacing = 4
    hbox.pack_start(table, :expand => false, :fill => false, :padding => 0)

    label = Gtk::Label.new("_Entry 1", :use_underline => true)
    table.attach(label, 0, 0, 1, 1)

    entry1 = Gtk::Entry.new
    table.attach(entry1, 1, 0, 1, 1)
    label.set_mnemonic_widget(entry1)

    entry2 = Gtk::Entry.new
    label = Gtk::Label.new("E_ntry 2", :use_underline => true)
    table.attach(entry2, 1, 1, 1, 1)

    button.signal_connect "clicked" do
      dialog = Gtk::Dialog.new(:parent => window,
                               :title => "Interactive Dialog",
                               :flags => [:modal, :destroy_with_parent],
                               :buttons => [["_OK", :ok],
                                            ["_Cancel", :cancel]]
                              )
      content_area = dialog.content_area
      local_hbox = Gtk::Box.new(:horizontal, 8)
      local_hbox.border_width = 8
      content_area.pack_start(local_hbox)

      image = Gtk::Image.new(:icon_name => "dialog-question", :size => :dialog)
      local_hbox.pack_start(image)

      local_table = Gtk::Grid.new
      local_table.row_spacing = 4
      local_table.column_spacing = 4
      local_hbox.pack_start(local_table, :expand => false, :fill => false, :padding => 0)

      label = Gtk::Label.new("_Entry 1", :use_underline => true)
      local_table.attach(label, 0, 0, 1, 1)

      local_entry1 = Gtk::Entry.new
      local_table.attach(local_entry1, 1, 0, 1, 1)
      label.set_mnemonic_widget(local_entry1)

      local_entry2 = Gtk::Entry.new
      label = Gtk::Label.new("E_ntry 2", :use_underline => true)
      local_table.attach(local_entry2, 1, 1, 1, 1)

      local_hbox.show_all
      response = dialog.run

      if response == :ok
        entry1.text = local_entry1.text
        entry2.text = local_entry2.text
      end

      dialog.destroy
    end

    if !window.visible?
      window.show_all
    else
      window.destroy
    end
    window
  end
end
