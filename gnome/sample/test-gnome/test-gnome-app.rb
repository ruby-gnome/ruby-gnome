#! /usr/local/bin/ruby
=begin header

  test-gnome-app.rb - testGNOME rewritten in Ruby/GNOME

  Rewritten by Unknown (GNOME 1.x version)
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

module Gtk # TODO: DELETE BEFORE RELEASE
  class Stock # StockItem ?
    COPY       = 'gtk-copy'        # GTK_STOCK_COPY
    CUT        = 'gtk-cut'         # GTK_STOCK_CUT
    NEW        = 'gtk-new'         # GTK_STOCK_NEW
    OPEN       = 'gtk-open'        # GTK_STOCK_OPEN
    PASTE      = 'gtk-paste'       # GTK_STOCK_PASTE
    PRINT      = 'gtk-print'       # GTK_STOCK_PRINT
    QUIT       = 'gtk-quit'        # GTK_STOCK_QUIT
    REDO       = 'gtk-redo'        # GTK_STOCK_REDO
    SAVE       = 'gtk-save'        # GTK_STOCK_SAVE
    SAVE_AS    = 'gtk-save-as'     # GTK_STOCK_SAVE_AS
    SPELLCHECK = 'gtk-spell-check' # GTK_STOCK_SPELL_CHECK
    UNDO       = 'gtk-undo'        # GTK_STOCK_UNDO
  end
  ICON_SIZE_BUTTON = 4 # GTK_ICON_SIZE_BUTTON
end

require 'gnome2'

class TestGnomeApp < Gnome::App
  AUTHORS = [
    "Richard Hestilow",
    "Federico Mena",
    "Eckehard Berns",
    "Havoc Pennington",
    "Miguel de Icaza",
    "Jonathan Blandford",
    "Carlos Perelló Marín",
    "Martin Baulig",
  ]

  DOCUMENTORS = [
    "Documentor1",
    "Documentor2",
  ]

  VERSION = "0.0.1"

  def initialize(normal, appname, title)
    super(appname, title)

    @normal = normal

    self.signal_connect("delete_event") { close_cb }

    file_menu = [
      Gnome::UIInfo::menu_close_item(Proc.new { close_cb }, nil),
      Gnome::UIInfo::menu_quit_item(Proc.new { quit_cb }, nil),
    ]

    help_menu = [
      # Gnome::UIInfo::help('testGnome'),  # TODO
      Gnome::UIInfo::menu_about_item(Proc.new { create_about }, nil),
    ]

    main_menu = [
      Gnome::UIInfo::menu_file_tree(file_menu),
      Gnome::UIInfo::menu_help_tree(help_menu)
    ]

    create_menus(main_menu)
  end

  private

  def close_cb
    self.destroy
    unless @normal
      Gtk::main_quit
    end
  end

  def quit_cb
    Gtk::main_quit
  end

  def create_about
    about = Gnome::About.new("GNOME Test Program", VERSION ,
                             "(C) 1998-2001 The Free Software Foundation",
                             "Program to display GNOME functions.",
                             AUTHORS,
                             DOCUMENTORS,
                             nil);
    about.show
  end
end # TestGnomeApp

if $0 == __FILE__
  puts "Don't call this file directly."
end

# Local variables:
# indent-tabs-mode: nil
# ruby-indent-level: 2
# End:
