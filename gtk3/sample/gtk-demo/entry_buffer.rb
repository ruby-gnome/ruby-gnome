# Copyright (c) 2008-2016 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
= Entry/Entry Buffer

Gtk::EntryBuffer provides the text content in a Gtk::Entry.
Applications can provide their own buffer implementation,
e.g. to provide secure handling for passwords in memory.
=end
class EntryBufferDemo
  def initialize(main_window)
    @window = Gtk::Window.new(:toplevel)
    @window.screen = main_window.screen
    @window.title = "Entry Buffer"
    @window.resizable = false

    vbox = Gtk::Box.new(:vertical, 5)
    vbox.margin = 5
    @window.add(vbox)

    label = Gtk::Label.new
    markup = "Entries share a buffer. Typing in one is reflected in the other."
    label.markup = markup
    vbox.pack_start(label, :expand => false, :fill => false, :padding => 0)

    # Create the buffer that will be shared
    buffer = Gtk::EntryBuffer.new

    # first entry
    entry = Gtk::Entry.new(buffer)
    vbox.pack_start(entry, :expand => false, :fill => false, :padding => 0)

    # second entry
    entry = Gtk::Entry.new(buffer)
    vbox.pack_start(entry, :expand => false, :fill => false, :padding => 0)
  end

  def run
    if !@window.visible?
      @window.show_all
    else
      @window.destroy
    end
  end
end
