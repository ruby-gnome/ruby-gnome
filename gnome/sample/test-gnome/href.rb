#!/usr/bin/env ruby
=begin header

  href.rb - HRef test rewritten in Ruby/GNOME

  Rewritten by Minoru Inachi <inachi@earth.interq.or.jp> (GNOME 1.x version)
               KUBO Takehiro <kubo@jiubao.org> (Ported to GNOME 2.0)

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

class HRefApp < TestGnomeApp
  def initialize
    super(true, "testGNOME", "HRef test")
    vbox = Gtk::VBox.new(false, 5)
    vbox.border_width = 5
    set_contents(vbox)

    @href = Gnome::HRef.new("http://www.gnome.org/", "Gnome Website")
    vbox.pack_start(@href, false, false, 0)

    wid = Gtk::HSeparator.new
    vbox.pack_start(wid, true, false, 0)

    wid = Gtk::Label.new(
        "The launch behaviour of the\nconfigured with the control center");
    vbox.pack_start(wid, true, false, 0)

    @url_ent = Gtk::Entry.new()
    @url_ent.set_text("http://www.gnome.org/")
    vbox.pack_start(@url_ent, true, true, 0)

    @label_ent = Gtk::Entry.new()
    @label_ent.set_text("Gnome Website")
    vbox.pack_start(@label_ent, true, true, 0)

    wid = Gtk::Button.new("set href props");
    wid.signal_connect("clicked") { href_cb }
    vbox.pack_start(wid, true, true, 0)

    show_all
  end

  private
  def href_cb
    url = @url_ent.text
    label = @label_ent.text
    if label.nil? || label.empty? then
      label = url
    end
    @href.url = url
    @href.label = label
  end
end

if $0 == __FILE__
  Gnome::Program.new("testGNOME", TestGnomeApp::VERSION, Gnome::ModuleInfo::LIBGNOMEUI)
  app = HrefApp.new
  app.signal_connect("destroy") { Gtk::main_quit }
  Gtk::main
end

# Local variables:
# indent-tabs-mode: nil
# ruby-indent-level: 2
# End:
