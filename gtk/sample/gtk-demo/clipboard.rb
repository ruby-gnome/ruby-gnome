# Copyright (c) 2003-2005 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
# $Id: clipboard.rb,v 1.3 2005/07/28 14:30:38 mutoh Exp $
=begin
= Clipboard

GtkClipboard is used for clipboard handling. This demo shows how to
copy and paste text to and from the clipboard.
=end

require 'common'

module Demo
  class Clipboard < Demo::BasicWindow
    def initialize
      super('Clipboard')

      unless Gtk.check_version?(2, 2, 0)
         add(Gtk::Label.new("This sample requires GTK+ 2.2.0 or later"))
         return
      end

      vbox = Gtk::VBox.new(false, 0)
      vbox.border_width = 8

      add(vbox)

      label = Gtk::Label.new(%Q["Copy" will copy the text\nin the entry to the clipboard])

      vbox.pack_start(label, false, false, 0)

      hbox = Gtk::HBox.new(false, 0)
      hbox.border_width = 8
      vbox.pack_start(hbox, false, false, 0)

      # Create the first entry
      entry = Gtk::Entry.new
      hbox.pack_start(entry, true, true, 0)

      # Create the button
      button = Gtk::Button.new(Gtk::Stock::COPY)
      hbox.pack_start(button, false, false, 0)
      button.signal_connect('clicked', entry) do |w, e|
        clipboard = e.get_clipboard(Gdk::Selection::CLIPBOARD)
        clipboard.text = e.text
      end

      label = Gtk::Label.new(%Q["Paste" will paste the text from the clipboard to the entry])
      vbox.pack_start(label, false, false, 0)

      hbox = Gtk::HBox.new(false, 4)
      hbox.border_width = 8
      vbox.pack_start(hbox, false, false, 0)

      # Create the second entry
      entry = Gtk::Entry.new
      hbox.pack_start(entry, true, true, 0)

      # Create the button
      button = Gtk::Button.new(Gtk::Stock::PASTE)
      hbox.pack_start(button, false, false, 0)
      button.signal_connect('clicked', entry) do |w, e|
        clipboard = e.get_clipboard(Gdk::Selection::CLIPBOARD)
        clipboard.request_text do |board, text, data|
          e.text = text
        end
      end

    end
  end
end

