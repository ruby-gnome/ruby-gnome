#! /usr/local/bin/ruby
=begin header

  font-picker.rb - FontPicker test rewritten in Ruby/GNOME

  Rewritten by Neil Conway <neilconway@rogers.com> (GNOME 1.x version)
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


class FontPickerApp < TestGnomeApp
  def initialize
    super(true, "testGNOME", "Font Picker")

    vbox = Gtk::VBox.new(false, 5)
    vbox.border_width = 5
    self.contents = vbox

    # Pixmap
    frPixmap = Gtk::Frame.new("Default Pixmap")
    vbox.pack_start(frPixmap, true, true, 0)
    vbox1 = Gtk::VBox.new(false, 0)
    frPixmap.add(vbox1)

    # GnomeFontPicker with pixmap
    fontpicker1 = Gnome::FontPicker.new()
    fontpicker1.border_width = 5
    vbox1.pack_start(fontpicker1, true, true, 0)
    lbPixmap = Gtk::Label.new("If you choose a font it will appear here")
    vbox1.pack_start(lbPixmap, true, true, 5)

    fontpicker1.signal_connect("font_set", lbPixmap) do |gfp, font_name, label|
      set_font(gfp, font_name, label)
    end

    # Font_Info
    frFontInfo = Gtk::Frame.new("Font Info")
    vbox.pack_start(frFontInfo, true, true, 0)
    vbox2 = Gtk::VBox.new(false, 0)
    vbox2.border_width = 5
    frFontInfo.add(vbox2)

    fontpicker2 = Gnome::FontPicker.new()

    # GnomeFontPicker with fontinfo
    hbox1 = Gtk::HBox.new(false, 5)
    vbox2.pack_start(hbox1, false, false, 0)
    ckUseFont = Gtk::CheckButton.new("Use Font in button with size")
    hbox1.pack_start(ckUseFont, true, true, 0)

    adj = Gtk::Adjustment.new(14, 5, 150, 1, 1, 1)
    adj.signal_connect("value_changed", fontpicker2) do |adj, gfp|
      value_changed(adj, gfp)
    end
    spUseFont = Gtk::SpinButton.new(adj, 1, 0)
    hbox1.pack_start(spUseFont, false, false, 0)
    # g_object_set_data (G_OBJECT (fontpicker2), "spUseFont", spUseFont);

    ckUseFont.signal_connect("toggled", fontpicker2) do |widget, gfp|
      usefont(widget, gfp)
    end

    ckShowSize = Gtk::CheckButton.new("Show font size")
    ckShowSize.active = true
    vbox2.pack_start(ckShowSize, false, false, 5)

    ckShowSize.signal_connect("toggled", fontpicker2) do |widget, gfp|
      showsize(widget, gfp)
    end

    fontpicker2.mode = Gnome::FontPicker::MODE_FONT_INFO
    vbox2.pack_start(fontpicker2, true, true, 0)

    lbFontInfo = Gtk::Label.new("If you choose a font it will appear here")
    vbox2.pack_start(lbFontInfo, true, true, 5)

    fontpicker2.signal_connect("font_set", lbFontInfo) do |gfp, font_name, label|
      set_font(gfp, font_name, label)
    end

    # User Widget
    frUser = Gtk::Frame.new("User Widget")
    vbox.pack_start(frUser, true, true, 0)
    vbox3 = Gtk::VBox.new(false, 0)
    frUser.add(vbox3)

    # GnomeFontPicker with User Widget
    fontpicker3 = Gnome::FontPicker.new()
    fontpicker3.mode = Gnome::FontPicker::MODE_USER_WIDGET

    hbox3 = Gtk::HBox.new(false, 0)
    hbox3.pack_start(Gtk::Image.new(Gtk::Stock::SPELL_CHECK, Gtk::IconSize::BUTTON), false, false, 5)
    hbox3.pack_start(Gtk::Label.new("This is an hbox with pixmap and text"), false, false, 5)
    fontpicker3.set_widget(hbox3)
    fontpicker3.border_width = 5
    vbox3.pack_start(fontpicker3, true, true, 0)

    lbUser = Gtk::Label.new("If you choose a font it will appear here")
    vbox3.pack_start(lbUser, true, true, 5)

    fontpicker3.signal_connect("font_set", lbUser) do |gfp, font_name, label|
      set_font(gfp, font_name, label)
    end

    self.show_all()
  end

  private
  def usefont(widget, gfp)
    show = gfp.get_property('use-font-in-label')
    gfp.set_property('use-font-in-label', !show)
  end

  def value_changed(adj, gfp)
    gfp.set_property('label-font-size', adj.value)
  end

  def showsize(togglebutton, gfp)
    #gfp.fi_set_show_size(togglebutton.active?)
    gfp.set_show_size(togglebutton.active?)
  end

  def set_font(gfp, font_name, label)
    printf("Font name: %s\n", font_name)
    label.set_text(font_name)
  end
end

if $0 == __FILE__
  Gnome::Program.new("testGNOME", TestGnomeApp::VERSION, Gnome::ModuleInfo::LIBGNOMEUI)
  app = FontPickerApp.new
  app.signal_connect("destroy") { Gtk::main_quit }
  Gtk::main
end

# Local variables:
# indent-tabs-mode: nil
# ruby-indent-level: 2
# End:
