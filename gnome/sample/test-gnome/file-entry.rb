#!/usr/bin/env ruby
=begin header

  file-entry.rb - FileEntry test rewritten in Ruby/GNOME

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

class FileEntryApp < TestGnomeApp
  def initialize
    super(true,"testGNOME", "File Entry")

    box = Gtk::VBox.new(false, 5)

    @entry = Gnome::FileEntry.new("Foo", "Bar")
    box.pack_start(@entry, false, false, 0)

    @l1 = Gtk::Label.new("File name: ")
    box.pack_start(@l1, false, false, 0)

    @l2 = Gtk::Label.new("File name(if exists only): ")
    box.pack_start(@l2, false, false, 0)

    but = Gtk::Button.new("Update file labels")
    but.signal_connect("clicked") { file_entry_update_files }
    box.pack_start(but, false, false, 0)

    but = Gtk::ToggleButton.new("Make browse dialog modal")
    but.signal_connect("toggled") { |w| file_entry_modal_toggle(w) }
    box.pack_start(but, false, false, 0)

    but = Gtk::ToggleButton.new("Directory only picker")
    but.signal_connect("toggled") { |w| file_entry_directory_toggle(w) }
    box.pack_start(but, false, false, 0)

    set_contents(box)
    show_all

  end

  private
  def file_entry_update_files
    p = @entry.get_full_path(false)
    pp = "File name: "
    pp += p unless p.nil?
    @l1.set_text(pp)

    p = @entry.get_full_path(true)
    pp = "File name(if exists only): "
    pp += p unless p.nil?
    @l2.set_text(pp)
  end

  private
  def file_entry_modal_toggle(w)
    @entry.set_modal(w.active?);
  end

  private
  def file_entry_directory_toggle(w)
    @entry.set_directory_entry(w.active?)
  end

end

if $0 == __FILE__
  Gnome::Program.new("testGNOME", TestGnomeApp::VERSION, Gnome::ModuleInfo::LIBGNOMEUI)
  app = FileEntryApp.new
  app.signal_connect("destroy") { Gtk::main_quit }
  Gtk::main
end

# Local variables:
# indent-tabs-mode: nil
# ruby-indent-level: 2
# End:
