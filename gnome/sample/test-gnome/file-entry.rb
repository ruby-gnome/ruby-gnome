=begin header

  file-entry.rb - FileEntry test rewritten in Ruby/GNOME

  Rewritten by Minoru Inachi <inachi@earth.interq.or.jp>

Original Copyright:
 
  Author : Richard Hestilow <hestgray@ionet.net>

  Copyright (C) 1998 Free Software Foundation

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

require 'sample'

class FileEntrySample < SampleApp
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
    @entry.set_directory(w.active?)
  end

end
