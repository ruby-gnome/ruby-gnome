#! /usr/local/bin/ruby
=begin header

  icon-entry.rb - IconEntry test rewritten in Ruby/GNOME

  Rewritten by Minoru Inachi <inachi@earth.interq.or.jp> (GNOME 1.x version)
               KUBO Takehiro <kubo@jiubao.org> (Ported to GNOME 2.0)

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

class IconEntryApp < TestGnomeApp
  def initialize
    super(true, "testGNOME", "Icon Entry")

    vbox = Gtk::VBox.new(false, 5)

    entry = Gnome::IconEntry.new("Foo", "Icon")
    vbox.pack_start(entry, true, true, 0)
    entry.show

    button = Gtk::Button.new("Update label below")
    vbox.pack_start(button, false, false, 0)
    button.show

    label = Gtk::Label.new("Nothing selected")
    vbox.pack_start(label, false, false, 0)
    label.show

    entry.signal_connect("changed") do |entry|
      file = entry.filename
      printf("Entry changed, new icon: %s\n", file.nil? ? "Nothing selected" : file)
    end
    button.signal_connect("clicked", entry) do |button, entry|
      file = entry.filename
      label.set_text(file.nil? ? "Nothing selected" : file)
    end

    set_contents(vbox)
    vbox.show
    show
  end
end

if $0 == __FILE__
  Gnome::Program.new("testGNOME", TestGnomeApp::VERSION, Gnome::ModuleInfo::LIBGNOMEUI)
  app = IconEntryApp.new
  app.signal_connect("destroy") { Gtk::main_quit }
  Gtk::main
end

# Local variables:
# indent-tabs-mode: nil
# ruby-indent-level: 2
# End:
