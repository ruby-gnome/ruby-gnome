=begin header

  color-picker.rb - ColorPicker test rewritten in Ruby/GNOME

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

class ColorPickerSample < SampleApp
  def initialize
    super(true, "testGNOME", "Color Picker")

    @table = Gtk::Table.new(3, 3, false)
    @table.border_width = Gnome::PAD_SMALL
    @table.set_row_spacings(Gnome::PAD_SMALL)
    @table.set_col_spacings(Gnome::PAD_SMALL)
    set_contents(@table)
    @table.show

    # Labels

    w = Gtk::Label.new("Dither")
    @table.attach(w, 1, 2, 0, 1, Gtk::FILL, Gtk::FILL, 0, 0)
    w.show

    w = Gtk::Label.new("No dither")
    @table.attach(w, 2, 3, 0, 1, Gtk::FILL, Gtk::FILL, 0, 0)
    w.show

    w = Gtk::Label.new("No alpha")
    @table.attach(w, 0, 1, 1, 2, Gtk::FILL, Gtk::FILL, 0, 0)
    w.show

    w = Gtk::Label.new("Alpha")
    @table.attach(w, 0, 1, 2, 3, Gtk::FILL, Gtk::FILL, 0, 0)
    w.show

    # Color pickers

    create_cp(true,  false, 1, 2, 1, 2);
    create_cp(false, false, 2, 3, 1, 2);
    create_cp(true,  true,  1, 2, 2, 3);
    create_cp(false, true,  2, 3, 2, 3);

    show
  end

  private
  def create_cp(dither, use_alpha, left, right, top, bottom)
    cp = Gnome::ColorPicker.new
    cp.set_dither(dither);
    cp.set_use_alpha(use_alpha)
    cp.set_d(1.0, 0.0, 1.0, 0.5)

    @table.attach(cp, left, right, top, bottom, 0, 0, 0, 0);
    cp.show
  end

end
