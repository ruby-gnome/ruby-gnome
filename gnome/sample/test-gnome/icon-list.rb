#!/usr/bin/env ruby
=begin header

  icon-list.rb - IconList test rewritten in Ruby/GNOME

  Rewritten by KUBO Takehiro <kubo@jiubao.org>

  Copyright (c) 2002-2003 Ruby-GNOME2 Project Team

Original Copyright:
 
  Authors : Richard Hestilow <hestgray@ionet.net> (GNOME 1.x version)
            Carlos Perelló Marín <carlos@gnome-db.org> (Ported to GNOME 2.0)

  Copyright (C) 1998-2001 Free Software Foundation

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2, or (at your option)
  any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
  02111-1307, USA.

=end

require 'test-gnome-app'
require 'gdk_pixbuf2'

class IconListApp < TestGnomeApp

  def initialize
    super(true, "testGNOME", "Icon List")

    sw = Gtk::ScrolledWindow.new(nil, nil)
    sw.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC)
    self.contents = sw
    sw.set_size_request(430, 300)
    sw.show()

    iconlist = Gnome::IconList.new(80, nil, Gnome::IconList::IS_EDITABLE)
    sw.add(iconlist)
    iconlist.signal_connect("select_icon") do |gii, n, event|
      printf("Icon %d selected", n)
      if (event)
        printf(" with event type %d\n", event.event_type)
      else
        printf("\n")
      end
    end
    iconlist.signal_connect("unselect_icon") do |gii, n, event|
      printf("Icon %d unselected", n)
      if (event)
        printf(" with event type %d\n", event.event_type)
      else
        printf("\n")
      end
    end

    iconlist.set_property(:can_focus, true)
    # pix = gdk_pixbuf_new_from_xpm_data ((const gchar **)bomb_xpm);
    pix = Gdk::Pixbuf.new(File.dirname(__FILE__) + '/bomb.xpm') # TODO

    iconlist.grab_focus()

    iconlist.freeze()

    30.times do
      iconlist.append_pixbuf(pix, "bomb.xpm", "Foo")
      iconlist.append_pixbuf(pix, "bomb.xpm", "Bar")
      iconlist.append_pixbuf(pix, "bomb.xpm", "LaLa")
    end

    iconlist.append("non-existant.png", "No Icon")

    iconlist.selection_mode = Gtk::SELECTION_MULTIPLE
    iconlist.thaw()
    iconlist.show()
    self.show()
  end
end

if $0 == __FILE__
  Gnome::Program.new("testGNOME", TestGnomeApp::VERSION, Gnome::ModuleInfo::LIBGNOMEUI)
  app = IconListApp.new
  app.signal_connect("destroy") { Gtk::main_quit }
  Gtk::main
end

# Local variables:
# indent-tabs-mode: nil
# ruby-indent-level: 2
# End:
