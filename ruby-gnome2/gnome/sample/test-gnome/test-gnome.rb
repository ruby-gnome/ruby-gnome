#!/usr/local/bin/ruby

=begin header

  test-gnome.rb - testGNOME rewritten in Ruby/GNOME

  Rewritten by Minoru Inachi <inachi@earth.interq.or.jp>

Original Copyright:
 
  testGNOME - program similar to testgtk which shows gnome lib functions.

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

require 'gtk'
require 'gdk_imlib'
require 'gnome'
require 'sample'

require 'calculator'
require 'clock'
require 'color-picker'
require 'paper-selector'
require 'dateedit'
require 'dialog'
require 'file-entry'
require 'pixmap-entry'
require 'icon-entry'
require 'number-entry'
require 'font-picker'
require 'less'
require 'href'

#
# Main Window
#
def create_main_window
  buttons = [
    [ "app-util/appbar/dialog-util", nil ],     # create_app_util
    [ "app-helper", nil ],                      # create_app_helper
    [ "calculator", CalculatorSample ],
    [ "canvas", nil ],                          # create_canvas
    [ "clock", ClockSample ],
    [ "color picker", ColorPickerSample ],
    [ "druid", nil ],                           # create_druid
    [ "guru", nil ],                            # create_guru
    [ "paper-sel", PaperSelectorSample ],
    [ "date edit", DateEditSample ],
    [ "dialog", DialogSample ],
    [ "file entry", FileEntrySample ],
    [ "pixmap entry", PixmapEntrySample ],
    [ "icon entry", IconEntrySample ],
    [ "number entry", NumberEntrySample ],
    [ "font picker", FontPickerSample ],        # create_font_picker
    [ "icon list", nil ],                       # create_icon_list
    [ "less", LessSample ],
    [ "pixmap", nil ],                          # create_pixmap
    [ "dentry edit", nil ],                     # create_dentry_edit
    [ "href", HRefSample ],
    [ "(Reload preferences)", nil ],            # gnome_preferences_load
    [ "prop box", nil ]                         # create_property_box
  ]
  nbuttons = buttons.size

  app = SampleApp.new(false, "testGNOME", "testGNOME")
  app.set_usize(250, 300)

  box1 = Gtk::VBox.new(false, 0)
  app.set_contents(box1)

  scrolled_window = Gtk::ScrolledWindow.new(nil, nil)
  scrolled_window.border_width(10)
  scrolled_window.set_policy(Gtk::POLICY_AUTOMATIC,
                             Gtk::POLICY_AUTOMATIC)
  # scrolled_window.vscrollbar.unset_flag(Gtk::CAN_FOCUS)
  box1.pack_start(scrolled_window, true, true, 0)

  box2 = Gtk::VBox.new(false, 0)
  box2.border_width(10)
  scrolled_window.add_with_viewport(box2);
  box2.set_focus_vadjustment(scrolled_window.get_vadjustment)

  for i in 0..(nbuttons-1)
    button = Gtk::Button.new(buttons[i][0])
    unless buttons[i][1].nil?
      button.signal_connect("clicked", buttons[i][1]) do |obj, sample_class|
        sample_class.new
      end
    else
      button.set_sensitive(false)
    end
    box2.pack_start(button)
  end

  app.show_all
end

def main
  create_main_window
  Gtk::main
end

main
