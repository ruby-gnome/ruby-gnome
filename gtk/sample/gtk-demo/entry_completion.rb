# Copyright (c) 2003-2005 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
# $Id: entry_completion.rb,v 1.3 2005/02/25 17:09:24 kzys Exp $
=begin
= Entry Completion (EntryCompletion)

GtkEntryCompletion provides a mechanism for adding support for
completion in GtkEntry.
=end

require 'common'

module Demo
  class EntryCompletion < Gtk::Dialog
    def initialize
      super('GtkEntryCompletion',
	    nil, # parent
	    0,
	    [Gtk::Stock::CLOSE, Gtk::Dialog::RESPONSE_NONE])

      self.resizable = false

      signal_connect('response') do
        self.destroy
      end

      vbox = Gtk::VBox.new(false, 5)
      self.vbox.pack_start(vbox, true, true)
      vbox.border_width = 5

      label = Gtk::Label.new
      label.markup = 'Completion demo, try writing <b>total</b> or <b>gnome</b> for example'
      vbox.pack_start(label, false, false)

      # Create our entry
      entry = Gtk::Entry.new
      vbox.pack_start(entry, false, false)

      # Create the completion object
      completion = Gtk::EntryCompletion.new

      # Assign the completion to the entry
      entry.completion = completion

      # Create a tree model and use it as the completion model
      completion.model = create_completion_model

      # Use model column 0 as the text column
      completion.text_column = 0
    end


    def create_completion_model
      store = Gtk::ListStore.new(String)
      %w(GNOME total totally).each do |word|
        iter = store.append
        iter[0] = word
      end

      store
    end
  end
end
