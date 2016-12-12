# Copyright (c) 2015-2016 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
= Entry/Entry Completion

GtkEntryCompletion provides a mechanism for adding support for
completion in GtkEntry.
=end
class EntryCompletionDemo
  def initialize(main_window)
    @window = Gtk::Window.new(:toplevel)
    @window.screen = main_window.screen
    @window.title = "Entry Completion"
    @window.resizable = true

    vbox = Gtk::Box.new(:vertical, 5)
    vbox.margin = 5
    @window.add(vbox)

    label = Gtk::Label.new
    markup = "Completion demo, try writing <b>total</b> or <b>gnome</b> for example."
    label.markup = markup
    vbox.pack_start(label, :expand => false, :fill => false, :padding => 0)

    entry = Gtk::Entry.new
    vbox.pack_start(entry, :expand => false, :fill => false, :padding => 0)

    completion = Gtk::EntryCompletion.new
    entry.completion = completion

    completion.model = create_completion_model
    completion.text_column = 0
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

  def create_completion_model
    store = Gtk::ListStore.new(String)
    %w(GNOME total totally).each do |word|
      iter = store.append
      iter[0] = word
    end

    store
  end
end
