#!/usr/bin/env ruby
=begin header

  test-gnome.rb - testGNOME rewritten in Ruby/GNOME

  Rewritten by Minoru Inachi <inachi@earth.interq.or.jp> (GNOME 1.x version)
               KUBO Takehiro <kubo@jiubao.org> (Ported to GNOME 2.0)

  Copyright (c) 2002-2003 Ruby-GNOME2 Project Team

Original Copyright:
 
  testGNOME - program similar to testgtk which shows gnome lib functions.

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

require 'app-helper'
require 'color-picker'
require 'dateedit'
require 'entry'
require 'file-entry'
require 'pixmap-entry'
require 'icon-entry'
require 'font-picker'
require 'href'
require 'icon-list'
require 'about'

class MainApp < TestGnomeApp
  def initialize
    buttons = [
      [ "app window", AppHelperApp],
      [ "color picker", ColorPickerApp ],
      [ "date edit", DateEditApp ],
      [ "entry", EntryApp],
      [ "file entry", FileEntryApp ],
      [ "pixmap entry", PixmapEntryApp ],
      [ "icon entry", IconEntryApp ],
      [ "font picker", FontPickerApp ],
      [ "href", HRefApp ],
      [ "icon list", IconListApp ],
      [ "about box", AboutApp ],
    ]

    super(false, "testGNOME", "testGNOME")
    self.set_default_size(200, 300)
    box1 = Gtk::VBox.new(false, 0)
    self.contents = box1
    box1.show()
    scrolled_window = Gtk::ScrolledWindow.new(nil, nil)
    scrolled_window.border_width = 10
    scrolled_window.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC)
    # scrolled_window.vscrollbar.set_property(:can_focus, false)
    box1.pack_start(scrolled_window, true, true, 0)
    scrolled_window.show()
    box2 = Gtk::VBox.new(false, 0)
    box2.border_width = 10
    scrolled_window.add_with_viewport(box2)
    box2.set_focus_vadjustment(scrolled_window.vadjustment)
    box2.show()
    buttons.each do |label, klass|
      button = Gtk::Button.new(label)
      unless klass.nil?
        button.signal_connect("clicked", klass) do |obj, klass|
          klass.new
        end
      else
        button.sensitive = false
      end
      box2.pack_start(button, true, true, 0)
      button.show()
    end

    self.show()
  end
end

Gnome::Program.new("testGNOME", TestGnomeApp::VERSION)
MainApp.new
Gtk::main

# Local variables:
# indent-tabs-mode: nil
# ruby-indent-level: 2
# End:
