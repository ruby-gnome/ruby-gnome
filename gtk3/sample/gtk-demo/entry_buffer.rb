# Copyright (c) 2008-2015 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
= Entry/Entry Buffer

Gtk::EntryBuffer provides the text content in a Gtk::Entry.
Applications can provide their own buffer implementation,
e.g. to provide secure handling for passwords in memory.
=end
module EntryBufferDemo

  def self.run_demo(main_window)
    window = Gtk::Window.new(:toplevel)
    window.screen = main_window.screen
    window.set_title("Entry Buffer")
    window.set_resizable(false)

    vbox = Gtk::Box.new(:vertical, 5)
    window.add(vbox)
    vbox.set_border_width(5)

    label = Gtk::Label.new
    label.set_markup("Entries share a buffer. Typing in one is reflected in the other.")
    vbox.pack_start(label, :expand => false, :fill => false, :padding => 0)

    buffer = Gtk::EntryBuffer.new

    entry = Gtk::Entry.new
    entry.buffer = buffer
    if !window.visible?
      window.show_all
    else
      window.destroy
    end
  end
end
