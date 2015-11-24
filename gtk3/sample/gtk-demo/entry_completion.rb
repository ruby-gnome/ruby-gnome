# Copyright (c) 2015 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
= Entry/Entry Completion

GtkEntryCompletion provides a mechanism for adding support for
completion in GtkEntry.
=end
module EntryCompletionDemo
  def self.run_demo(main_window)
    window = Gtk::Window.new(:toplevel)
    window.screen = main_window.screen
    window.set_title("Entry Completion")
    window.set_resizable(true)

    vbox = Gtk::Box.new(:vertical, 5)
    window.add(vbox)
    vbox.set_border_width(5)

    label = Gtk::Label.new
    label.set_markup("Completion demo, try writing <b>total</b> or <b>gnome</b> for example.")
    vbox.pack_start(label, :expand => false, :fill => false, :padding => 0)

    entry = Gtk::Entry.new
    vbox.pack_start(entry, :expand => false, :fill => false, :padding => 0)

    completion = Gtk::EntryCompletion.new
    entry.completion = completion

    completion.set_model(create_completion_model)
    completion.set_text_column(0)

    if !window.visible?
      window.show_all
    else
      window.destroy
    end
    window
  end

  def self.create_completion_model
    store = Gtk::ListStore.new(String)
    %w(GNOME total totally).each do |word|
      iter = store.append
      iter[0] = word
    end

    store
  end
end
