# -*- coding: utf-8 -*-
=begin header

  labels.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team

  $Id: labels.rb,v 1.7 2005/07/17 16:55:27 mutoh Exp $

=end

require 'sample'

class LabelSample < SampleWindow
  def initialize
    super("Gtk::Label")
    set_border_width(5)

    vbox = Gtk::VBox::new(false, 5)
    hbox = Gtk::HBox::new(false, 5)
    hbox.pack_start(vbox, false, false, 0)

    create_frame("Normal Label",
		 "This is a Normal label",
		 vbox)
    create_frame("Multi-line Label",
		 "This is a Multi-line label.\nSecond line\nThird line",
		 vbox)
    create_frame("Left Justified Label",
		 "This is a Left-Justified\nMulti-line label.\nThird      line",
		 vbox) do |label|
      label.justify = Gtk::JUSTIFY_LEFT
    end
    create_frame("Right Justified Label",
		 "This is a Right-Justified\nMulti-line label.\nFourth line, (j/k)",
		 vbox) do |label|
      label.justify = Gtk::JUSTIFY_RIGHT
    end

    vbox = Gtk::VBox::new(false, 5)
    hbox.pack_start(vbox, false, false, 0)

    create_frame("Line wrapped label",
		 "This is an example of a line-wrapped label.  It should not be taking up the entire             width allocated to it, but automatically wraps the words to fit.  The time has come, for all good men, to come to the aid of their party.  The sixth sheik's six sheep's sick.\n     It supports multiple paragraphs correctly, and  correctly   adds many          extra  spaces. ",
		 vbox) do |label|
      label.wrap = true
    end
    create_frame("Filled, wrapped label",
		 "This is an example of a line-wrapped, filled label.  It should be taking up the entire              width allocated to it.  Here is a seneance to prove my point.  Here is another sentence. Here comes the sun, do de do de do.\n    This is a new paragraph.\n    This is another newer, longer, better paragraph.  It is coming to an end, unfortunately.",
		 vbox) do |label|
      label.justify = Gtk::JUSTIFY_FILL
      label.wrap = true
    end
    create_frame("Underlined label",
		 "This label is underlined!\nThis one is underlined in 日本語の入用quite a funky fashion",
		 vbox) do |label|
      label.justify = Gtk::JUSTIFY_LEFT
      label.pattern = "_________________________ _ _________ _ _____ _ __ __  ___ ____ _____"
    end

    add(hbox)
  end

  def create_frame(title, label_str, vbox)
    frame = Gtk::Frame::new(title)
    frame.show
    label = Gtk::Label::new(label_str)
    yield(label) if iterator?
    frame.add(label)
    vbox.pack_start(frame, false, false, 0)
  end

  private :create_frame
end
