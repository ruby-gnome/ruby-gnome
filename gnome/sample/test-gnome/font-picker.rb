=begin header

  font-picker.rb - FontPicker test rewritten in Ruby/GNOME

  Rewritten by Neil Conway <neilconway@rogers.com>

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

class FontPickerSample < SampleApp
  def initialize
    super(true, "testGNOME", "Font Picker")

    vbox = Gtk::VBox.new(false, 5)
    vbox.border_width = 5
    set_contents(vbox)

    fr_pixmap = Gtk::Frame.new("Default Pixmap")
    vbox.pack_start(fr_pixmap, true, true, 0)

    vbox1 = Gtk::VBox.new(false, 0)
    fr_pixmap.add(vbox1)
    fp1 = Gnome::FontPicker.new
    fp1.border_width = 5
    vbox1.pack_start(fp1, true, true, 0)
    label_pixmap = Gtk::Label.new("If you choose a font it will appear here")
    vbox1.pack_start(label_pixmap, true, true, 5)
    fp1.signal_connect('font_set', label_pixmap) do |*a| set_font(*a) end

    fr_font_info = Gtk::Frame.new
    vbox.pack_start(fr_font_info, false, false, 0)

    vbox2 = Gtk::VBox.new(false, 0)
    vbox2.border_width = 5
    fr_font_info.add(vbox2)

    hbox1 = Gtk::HBox.new(false, 5)
    vbox2.pack_start(hbox1)
    use_font = Gtk::CheckButton.new("Use Font in button with size")
    hbox1.pack_start(use_font, true, true, 0)
    
    adj = Gtk::Adjustment.new(14, 5, 150, 1, 1, 1)
    spin_use_font = Gtk::SpinButton.new(adj, 1, 0)
    hbox1.pack_start(spin_use_font, false, false, 0)

    show_size = Gtk::CheckButton.new("Show font size")
    show_size.active = true
    vbox2.pack_start(show_size, false, false, 5)

    fp2 = Gnome::FontPicker.new
    fp2.set_mode(Gnome::FontPicker::MODE_FONT_INFO)
    vbox2.pack_start(fp2, true, true, 0)

    use_font.signal_connect("toggled", fp2) do |w, gfp|
      gfp.fi_set_use_font_in_label(! gfp.get_use_font_in_label,
                                     gfp.get_use_font_in_label_size)
    end

    show_size.signal_connect("toggled") do
      ;
    end

    adj.signal_connect("value-changed", fp2) do |adj, gfp|
      gfp.fi_set_use_font_in_label( gfp.get_use_font_in_label, adj.value)
    end

    label_font_info = Gtk::Label.new("If you choose a font it will appear here")
    vbox2.pack_start(label_font_info, true, true, 5)

    fr_user = Gtk::Frame.new("User Widget")
    vbox.pack_start(fr_user, true, true, 0)
    vbox3 = Gtk::VBox.new(false, 0)
    fr_user.add(vbox3)

    fp3 = Gnome::FontPicker.new
    fp3.set_mode(Gnome::FontPicker::MODE_USER_WIDGET)
    hbox3 = Gtk::HBox.new(false, 0)
    hbox3.pack_start(
      Gnome::Stock.new(Gnome::Stock::PIXMAP_SPELLCHECK), false, false, 5)
    hbox3.pack_start(
      Gtk::Label.new("This is an hbox with pixmap and text"), false, false, 5)
    fp3.uw_set_widget(hbox3)
    fp3.border_width = 5
    vbox3.pack_start(fp3, true, true, 0)

    label_user = Gtk::Label.new("If you choose a font it will appear here")
    vbox3.pack_start(label_user, true, true, 5)
    fp3.signal_connect("font-set") do
      ;
    end

    show_all
  end

  def set_font(fp, font_name, label)
    puts "Font name: #{font_name}"
    label.set_text(font_name)
  end
end
