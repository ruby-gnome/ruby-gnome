=begin header

  entry.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team

  $Id: entry.rb,v 1.5 2003/02/01 16:46:23 mutoh Exp $

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
    set_border_width(0)

    box1 = Gtk::VBox::new(false, 0)
    add(box1)

    box2 = Gtk::VBox::new(false, 10)
    box2.set_border_width(10)
    box1.pack_start(box2, true, true, 0)

    entry = Gtk::Entry::new()
    entry.set_text("hello world")
    entry.select_region(0, 5)
    box2.pack_start(entry, true, true, 0)

    cb = Gtk::Combo::new()
    cb.set_popdown_strings(cbitems)
    cb.entry.set_text("hello world")
    cb.entry.select_region(0, -1)
    box2.pack_start(cb, true, true, 0)

    editable_check = Gtk::CheckButton::new("Editable")
    box2.pack_start(editable_check, false, true, 0)
    editable_check.signal_connect("toggled") do
      entry.set_editable(editable_check.active?)
    end
    editable_check.set_active(true)

    visible_check = Gtk::CheckButton::new("Visible")
    box2.pack_start(visible_check, false, true, 0)
    visible_check.signal_connect("toggled") do
      entry.set_visibility(visible_check.active?)
    end
    visible_check.set_active(true)

    sensitive_check = Gtk::CheckButton::new("Sensitive")
    box2.pack_start(sensitive_check, false, true, 0)
    sensitive_check.signal_connect("toggled") do
      entry.set_sensitive(sensitive_check.active?)
    end
    sensitive_check.set_active(true)

    separator = Gtk::HSeparator::new()
    box1.pack_start(separator, false, true, 0)

    box2 = Gtk::VBox::new(false, 10)
    box2.set_border_width(10)
    box1.pack_start(box2, false, true, 0)

    button = Gtk::Button::new("close")
    button.signal_connect("clicked") do
      destroy
    end
    box2.pack_start(button, true, true, 0)
    button.set_flags(Gtk::Widget::CAN_DEFAULT)
    button.grab_default
  end
end
