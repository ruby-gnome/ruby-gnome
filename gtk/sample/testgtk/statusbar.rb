=begin header

  statusbar.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team

  Rewritten by Hiroshi IGARASHI <igarashi@ueda.info.waseda.ac.jp>
  $Date: 2003/02/01 16:46:23 $
  $Id: statusbar.rb,v 1.5 2003/02/01 16:46:23 mutoh Exp $

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

class StatusbarSample < SampleWindow
  def initialize
    super("statusbar")
    set_border_width(0)

    @counter = 1

    box1 = Gtk::VBox.new(false, 0)
    add(box1)
    box1.show

    box2 = Gtk::VBox.new(false, 10)
    box2.set_border_width(10)
    box1.pack_start(box2, true, true, 0)
    box2.show

    @statusbar = statusbar = Gtk::Statusbar.new
    box1.pack_end(statusbar, true, true, 0)
    statusbar.show
    statusbar.signal_connect("text_popped") do |o, mid, text|
      popped(mid, text)
    end
    statusbar.signal_connect("text_pushed") do |o, mid, text|
      pushed(mid, text)
    end
#
# GtkWidget* gtk_widget_new  (guint type, ...);
#
    button = Gtk::Button.new("push something")
    button.show
    box2.pack_start(button, true, true, 0)
    button.signal_connect("clicked") do push end

    button = Gtk::Button.new("pop")
    button.show
    box2.pack_start(button, true, true, 0)
    button.signal_connect("clicked") do pop end

    button = Gtk::Button.new("steal #4")
    button.show
    box2.pack_start(button, true, true, 0)
    button.signal_connect("clicked") do steal end

    button = Gtk::Button.new("test contexts")
    button.show
    box2.pack_start(button, true, true, 0)
    button.signal_connect("clicked") do contexts end

    separator = Gtk::HSeparator.new
    box1.pack_start(separator, false, true, 0)
    separator.show

    box2 = Gtk::VBox.new(false, 10)
    box2.set_border_width(10)
    box1.pack_start(box2, false, true, 0)
    box2.show

    button = Gtk::Button.new("close")
    button.signal_connect("clicked") do destroy end
    box2.pack_start(button, true, true, 0)
    button.set_flags(Gtk::Widget::CAN_DEFAULT)
    button.grab_default
    button.show
  end
  private
  def push
    text = sprintf("something %d", @counter)
    @counter += 1    
    @statusbar.push(1, text)
  end
  def pop
    @statusbar.pop(1)
  end
  def steal
    @statusbar.remove(1, 4)
  end
  def popped(mid, text)
    if text then
      @counter = 1
    end
  end
  def pushed(mid, text)
    # do nothing... as of gtk 1.2.6
  end
  def contexts
    string = "any context"
    printf("GtkStatusBar: context=\"%s\", context_id=%d\n",
           string,
           @statusbar.get_context_id(string))
    
    string = "idle messages"
    printf("GtkStatusBar: context=\"%s\", context_id=%d\n",
           string,
           @statusbar.get_context_id(string))
  
    string = "some text"
    printf("GtkStatusBar: context=\"%s\", context_id=%d\n",
           string,
           @statusbar.get_context_id(string))

    string = "hit the mouse"
    printf("GtkStatusBar: context=\"%s\", context_id=%d\n",
           string,
           @statusbar.get_context_id(string))

    string = "hit the mouse2"
    printf("GtkStatusBar: context=\"%s\", context_id=%d\n",
           string,
           @statusbar.get_context_id(string))
  end
end
