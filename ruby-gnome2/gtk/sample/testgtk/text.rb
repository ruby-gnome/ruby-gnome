=begin header

  text.rb - a part of testgtk.c rewritten in ruby-gtk

  Rewritten by Hiroshi IGARASHI <igarashi@ueda.info.waseda.ac.jp>
  $Id: text.rb,v 1.1 2002/05/19 12:29:44 mutoh Exp $

Original Copyright:
 
  GTK - The GIMP Toolkit
  Copyright (C) 1995-1997 Peter Mattis, Spencer Kimball and Josh MacDonald
 
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.
 
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.
 
  You should have received a copy of the GNU Library General Public
  License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place - Suite 330,
  Boston, MA 02111-1307, USA.

=end

require 'sample'

#
# GtkText
#
class TextSample < SampleWindow

  def toggle_editable(checkbutton)
    @text.set_editable(checkbutton.active?)
  end

  def toggle_word_wrap(checkbutton)
    @text.set_word_wrap(checkbutton.active?)
  end

  def insert_random
    for i in 0..9
      c = ?A + Kernel.rand(?Z - ?A)
      @text.set_point(Kernel.rand(@text.get_length))
      @text.insert(nil, nil, nil, sprintf("%c", c))
    end
  end

  TextColors = [
    [ [ 0x0000, 0x0000, 0x0000 ], "black" ],
    [ [ 0xFFFF, 0xFFFF, 0xFFFF ], "white" ],
    [ [ 0xFFFF, 0x0000, 0x0000 ], "red" ],
    [ [ 0x0000, 0xFFFF, 0x0000 ], "green" ],
    [ [ 0x0000, 0x0000, 0xFFFF ], "blue" ], 
    [ [ 0x0000, 0xFFFF, 0xFFFF ], "cyan" ],
    [ [ 0xFFFF, 0x0000, 0xFFFF ], "magenta" ],
    [ [ 0xFFFF, 0xFFFF, 0x0000 ], "yellow" ]
  ]

  def initialize
    super("test")
    set_name("text window")
    set_usize(500, 500)
    set_policy(true, true, false)

    signal_connect("destroy") do
      destroy
    end

    box1 = Gtk::VBox.new(false, 0)
    add(box1)
    box1.show

    box2 = Gtk::VBox.new(false, 10)
    box2.border_width = 10
    box1.pack_start(box2, true, true, 0)
    box2.show

    scrolled_window = Gtk::ScrolledWindow.new(nil, nil)
    box2.pack_start(scrolled_window, true, true, 0)
    scrolled_window.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_ALWAYS)
    scrolled_window.show

    @text = text = Gtk::Text.new(nil, nil)
    text.set_editable(true)
    scrolled_window.add(@text)
    text.grab_focus
    text.show

    text.freeze
    
    font = Gdk::Font.font_load("-adobe-courier-medium-r-normal--*-120-*-*-*-*-*-*")

    TextColors.each do |text_color|
      text.insert(font, nil, nil, text_color[1])
      text.insert(font, nil, nil, "\t")

      TextColors.each do |text_color2|
	text.insert(font,
		    Gdk::Color.new(*text_color2[0]),
		    Gdk::Color.new(*text_color[0]), "XYZ")
      end
      text.insert(nil, nil, nil, "\n")
    end

    # The Text widget will reference count the font, so we
    # unreference it here
    #
    #      font.unref

    begin
      buffer = nil
      File.open("testgtk.rb", "r") do |infile|
	buffer = infile.read
      end
      text.insert(nil, nil, nil, buffer)
      text.thaw
    end
    
    hbox = Gtk::HButtonBox.new
    box2.pack_start(hbox, false, false, 0)
    hbox.show

    check = Gtk::CheckButton.new("Editable")
    hbox.pack_start(check, false, false, 0)
    check.signal_connect("toggled") do toggle_editable(check) end
    check.set_active(true)
    check.show

    check2 = Gtk::CheckButton.new("Wrap Words")
    hbox.pack_start(check2, false, true, 0)
    check2.signal_connect("toggled") do toggle_word_wrap(check2) end
    check2.set_active(false)
    check2.show

    separator = Gtk::HSeparator.new
    box1.pack_start(separator, false, true, 0)
    separator.show


    box2 = Gtk::VBox.new(false, 10)
    box2.border_width = 10
    box1.pack_start(box2, false, true, 0)
    box2.show


    button = Gtk::Button.new("insert random")
    button.signal_connect("clicked") do insert_random end
    box2.pack_start(button, true, true, 0)
    button.show

    button = Gtk::Button.new("close")
    button.signal_connect("clicked") do destroy end
    box2.pack_start(button, true, true, 0)
    button.set_flags(Gtk::Widget::CAN_DEFAULT)
    button.grab_default
    button.show
  end
end
