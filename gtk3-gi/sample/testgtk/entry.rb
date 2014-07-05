=begin header

  entry.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team

  $Id: entry.rb,v 1.6 2005/07/17 16:55:27 mutoh Exp $

=end

require 'sample'

class EntrySample < SampleWindow
  def initialize
    cbitems = [
      "item0",
      "item1 item1",
      "item2 item2 item2",
      "item3 item3 item3 item3",
      "item4 item4 item4 item4 item4",
      "item5 item5 item5 item5 item5 item5",
      "item6 item6 item6 item6 item6",
      "item7 item7 item7 item7",
      "item8 item8 item8",
      "item9 item9"
    ]

    super("entry")

    box = Gtk::VBox.new(false, 5)
    add(box)

    entry = Gtk::Entry.new
    entry.text = "hello world"
    entry.select_region(0, 5)
    box.add(entry)

    cb = Gtk::Combo.new
    cb.set_popdown_strings(cbitems)
    cb.entry.text = "hello world"
    cb.entry.select_region(0, -1)
    box.add(cb)

    editable_check = Gtk::CheckButton.new("Editable")
    box.add(editable_check)
    editable_check.signal_connect("toggled"){
      entry.editable = editable_check.active?
    }
    editable_check.active = true

    visible_check = Gtk::CheckButton.new("Visible")
    box.add(visible_check)
    visible_check.signal_connect("toggled"){
      entry.visibility = visible_check.active?
    }
    visible_check.active = true

    sensitive_check = Gtk::CheckButton.new("Sensitive")
    box.add(sensitive_check)
    sensitive_check.signal_connect("toggled"){
      entry.sensitive = sensitive_check.active?
    }
    sensitive_check.active = true

    box.pack_start(Gtk::HSeparator.new, false, true, 5)

    button = Gtk::Button.new("close")
    button.signal_connect("clicked"){ destroy }
    
    box.add(button)

  end
end
