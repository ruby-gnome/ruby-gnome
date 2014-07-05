=begin header

  statusbar.rb - a part of testgtk.c rewritten in Ruby/GTK2

  Copyright (C) 2002-2005 Ruby-GNOME2 Project Team

  Rewritten by Hiroshi IGARASHI <igarashi@ueda.info.waseda.ac.jp>
  $Date: 2005/07/21 17:47:19 $
  $Id: statusbar.rb,v 1.6 2005/07/21 17:47:19 mutoh Exp $

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

    set_border_width(5)

    @counter = 1

    vbox = Gtk::VBox.new(false, 5)
    add(vbox)

    statusbar = Gtk::Statusbar.new

    vbox.pack_end(statusbar, true, true, 0)
    statusbar.signal_connect("text_popped"){|o, mid, text|
      @counter = 1 if text
      puts "poped"      
    }
    statusbar.signal_connect("text_pushed"){|o, mid, text|
      # do nothing... as of gtk 1.2.6
      puts "pushed"
    }

    button_push = Gtk::Button.new("push something")
    vbox.add(button_push)
    button_push.signal_connect("clicked"){
      @counter += 1    
      statusbar.push(1, "something #{@counter}")
    }

    button_pop = Gtk::Button.new("pop")
    vbox.add(button_pop)
    button_pop.signal_connect("clicked"){
      statusbar.pop(1)
    }
    button_steal = Gtk::Button.new("steal #4")
    vbox.add(button_steal)
    button_steal.signal_connect("clicked"){
      statusbar.remove(1, 4)
    }
    button_context = Gtk::Button.new("test contexts")
    vbox.add(button_context)
    button_context.signal_connect("clicked"){
      ["any context", "idle messages", "some text", 
	"hit the mouse", "hit the mouse2"].each do |str|
	puts "Gtk::StatusBar: context=\"#{str}\", context_id=#{statusbar.get_context_id(str)}"
      end
    }
    vbox.add(Gtk::HSeparator.new)

    button_close = Gtk::Button.new("close")
    button_close.signal_connect("clicked"){destroy}
    vbox.add(button_close)
  end
end
