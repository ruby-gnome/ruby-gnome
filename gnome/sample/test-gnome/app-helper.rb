#! /usr/local/bin/ruby
=begin header

  app-helper.rb - App Helper test rewritten in Ruby/GNOME

  Rewritten by KUBO Takehiro <kubo@jiubao.org>

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

class AppHelperApp < Gnome::App

  # Event handler for canvas items in the explicit popup menu demo
  def item_event(item, event, popup)
    return false unless (event.event_type == Gdk::Event::BUTTON_PRESS) && (event.button == 3)
    popup.do_popup(nil, event, item, nil)
    true
  end

  def initialize
    super("testGNOME", "GnomeAppHelper test")

    # Used as a callback for menu items in the GnomeAppHelper test;
    # just prints the string contents of the data pointer.
    item_activated = Proc.new do |widget, data|
      printf("%s activated\n", data)
    end

    # Menu definitions for the GnomeAppHelper test
    helper_file_menu = [
      [ Gnome::App::UI_ITEM, "_New", "Create a new file", item_activated, "file/new",
        Gnome::App::PIXMAP_STOCK, Gtk::Stock::NEW, ?n, Gdk::Window::CONTROL_MASK, nil ],
      [ Gnome::App::UI_ITEM, "_Open...", "Open an existing file", item_activated, "file/open",
        Gnome::App::PIXMAP_STOCK, Gtk::Stock::OPEN, ?o, Gdk::Window::CONTROL_MASK, nil ],
      [ Gnome::App::UI_ITEM, "_Save", "Save the current file", item_activated, "file/save",
        Gnome::App::PIXMAP_STOCK, Gtk::Stock::SAVE, ?s, Gdk::Window::CONTROL_MASK, nil ],
      [ Gnome::App::UI_ITEM, "Save _as...", "Save the current file with a new name", item_activated, "file/save as",
        Gnome::App::PIXMAP_STOCK, Gtk::Stock::SAVE_AS, 0, 0, nil ],
      Gnome::UIInfo::separator,
      [ Gnome::App::UI_ITEM, "_Print...", "Print the current file", item_activated, "file/print",
        Gnome::App::PIXMAP_STOCK, Gtk::Stock::PRINT, ?p, Gdk::Window::CONTROL_MASK, nil ],
      Gnome::UIInfo::separator,
      Gnome::UIInfo::menu_close_item(item_activated, "file/close"),
      Gnome::UIInfo::menu_quit_item(item_activated, "file/exit"),
    ]

    helper_edit_menu = [
      [ Gnome::App::UI_ITEM, "_Undo", "Undo the last operation", item_activated, "edit/undo",
        Gnome::App::PIXMAP_STOCK, Gtk::Stock::UNDO, ?z, Gdk::Window::CONTROL_MASK, nil ],
      [ Gnome::App::UI_ITEM, "_Redo", "Redo the last undo-ed operation", item_activated, "edit/redo",
        Gnome::App::PIXMAP_STOCK, Gtk::Stock::REDO, 0, 0, nil ],
      Gnome::UIInfo::separator,
      [ Gnome::App::UI_ITEM, "Cu_t", "Cut the selection to the clipboard", item_activated, "edit/cut",
        Gnome::App::PIXMAP_STOCK, Gtk::Stock::CUT, ?x, Gdk::Window::CONTROL_MASK, nil ],
      [ Gnome::App::UI_ITEM, "_Copy", "Copy the selection to the clipboard", item_activated, "edit/copy",
        Gnome::App::PIXMAP_STOCK, Gtk::Stock::COPY, ?c, Gdk::Window::CONTROL_MASK, nil ],
      [ Gnome::App::UI_ITEM, "_Paste", "Paste the contents of the clipboard", item_activated, "edit/paste",
        Gnome::App::PIXMAP_STOCK, Gtk::Stock::PASTE, ?v, Gdk::Window::CONTROL_MASK, nil ],
    ]

    helper_style_radio_items = [
      [ Gnome::App::UI_ITEM, "_10 points", nil, item_activated, "style/10 points",
        Gnome::App::PIXMAP_NONE, nil, 0, 0, nil ],
      [ Gnome::App::UI_ITEM, "_20 points", nil, item_activated, "style/20 points",
        Gnome::App::PIXMAP_NONE, nil, 0, 0, nil ],
      [ Gnome::App::UI_ITEM, "_30 points", nil, item_activated, "style/30 points",
        Gnome::App::PIXMAP_NONE, nil, 0, 0, nil ],
      [ Gnome::App::UI_ITEM, "_40 points", nil, item_activated, "style/40 points",
        Gnome::App::PIXMAP_NONE, nil, 0, 0, nil ],
    ]

    helper_style_menu = [
      [ Gnome::App::UI_TOGGLEITEM, "_Bold", "Make the selection bold", item_activated, "style/bold",
        Gnome::App::PIXMAP_NONE, nil, 0, 0, nil ],
      [ Gnome::App::UI_TOGGLEITEM, "_Italic", "Make the selection italic", item_activated, "style/bold",
        Gnome::App::PIXMAP_NONE, nil, 0, 0, nil ],
      Gnome::UIInfo::separator,
      Gnome::UIInfo::radiolist(helper_style_radio_items),
    ]

    helper_help_menu = [
      [ Gnome::App::UI_ITEM, "_About...", "Displays information about the program", item_activated, "help/about",
        Gnome::App::PIXMAP_STOCK, Gnome::Stock::ABOUT, 0, 0, nil ],
    ]

    helper_main_menu = [
      [ Gnome::App::UI_SUBTREE, "_File", "File operations", helper_file_menu, nil,
        Gnome::App::PIXMAP_NONE, nil, 0, 0, nil ],
      [ Gnome::App::UI_SUBTREE, "_Edit", "Editing commands", helper_edit_menu, nil,
        Gnome::App::PIXMAP_NONE, nil, 0, 0, nil ],
      [ Gnome::App::UI_SUBTREE, "_Style", "Style settings", helper_style_menu, nil,
        Gnome::App::PIXMAP_NONE, nil, 0, 0, nil ],
      [ Gnome::App::UI_SUBTREE, "_Help", "Help on the program", helper_help_menu, nil,
        Gnome::App::PIXMAP_NONE, nil, 0, 0, nil ],
    ]

    # Toolbar definition for the GnomeAppHelper test
    helper_toolbar_radio_items = [
      [ Gnome::App::UI_ITEM, "Red", "Set red color", item_activated, "toolbar/red",
        Gnome::App::PIXMAP_STOCK, Gnome::Stock::BOOK_RED, 0, 0, nil ],
      [ Gnome::App::UI_ITEM, "Green", "Set green color", item_activated, "toolbar/green",
        Gnome::App::PIXMAP_STOCK, Gnome::Stock::BOOK_GREEN, 0, 0, nil ],
      [ Gnome::App::UI_ITEM, "Blue", "Set blue color", item_activated, "toolbar/blue",
          Gnome::App::PIXMAP_STOCK, Gnome::Stock::BOOK_BLUE, 0, 0, nil ],
      [ Gnome::App::UI_ITEM, "Yellow", "Set yellow color", item_activated, "toolbar/yellow",
        Gnome::App::PIXMAP_STOCK, Gnome::Stock::BOOK_YELLOW, 0, 0, nil ],
    ]

    helper_toolbar = [
      [ Gnome::App::UI_ITEM, "New", "Create a new file", item_activated, "toolbar/new",
        Gnome::App::PIXMAP_STOCK, Gtk::Stock::NEW, 0, 0, nil ],
      [ Gnome::App::UI_ITEM, "Open", "Open an existing file", item_activated, "toolbar/open",
        Gnome::App::PIXMAP_STOCK, Gtk::Stock::OPEN, 0, 0, nil ],
      [ Gnome::App::UI_ITEM, "Save", "Save the current file", item_activated, "toolbar/save",
        Gnome::App::PIXMAP_STOCK, Gtk::Stock::SAVE, 0, 0, nil ],
      [ Gnome::App::UI_ITEM, "Print", "Print the current file", item_activated, "toolbar/print",
        Gnome::App::PIXMAP_STOCK, Gtk::Stock::PRINT, 0, 0, nil ],
      Gnome::UIInfo::separator,
      [ Gnome::App::UI_ITEM, "Undo", "Undo the last operation", item_activated, "toolbar/undo",
        Gnome::App::PIXMAP_STOCK, Gtk::Stock::UNDO, 0, 0, nil ],
      [ Gnome::App::UI_ITEM, "Redo", "Redo the last undo-ed operation", item_activated, "toolbar/redo",
        Gnome::App::PIXMAP_STOCK, Gtk::Stock::REDO, 0, 0, nil ],
      Gnome::UIInfo::separator,
      [ Gnome::App::UI_ITEM, "Cut", "Cut the selection to the clipboard", item_activated, "toolbar/cut",
        Gnome::App::PIXMAP_STOCK, Gtk::Stock::CUT, 0, 0, nil ],
      [ Gnome::App::UI_ITEM, "Copy", "Copy the selection to the clipboard", item_activated, "toolbar/copy",
        Gnome::App::PIXMAP_STOCK, Gtk::Stock::COPY, 0, 0, nil ],
      [ Gnome::App::UI_ITEM, "Paste", "Paste the contents of the clipboard", item_activated, "toolbar/paste",
        Gnome::App::PIXMAP_STOCK, Gtk::Stock::PASTE, 0, 0, nil ],
      Gnome::UIInfo::separator,
      Gnome::UIInfo::radiolist(helper_toolbar_radio_items),
    ]

    # These three functions insert some silly text in the GtkEntry specified in the user data
    insert_red = Proc.new do |widget, entry, for_widget|
      entry.insert_text("red book ", entry.position)
    end

    insert_green  = Proc.new do |widget, entry, for_widget|
      entry.insert_text("green book ", entry.position)
    end

    insert_blue  = Proc.new do |widget, entry, for_widget|
      entry.insert_text("blue book ", entry.position)
    end

    # Shared popup menu definition for the GnomeAppHelper test
    helper_shared_popup_dup = [
      [ Gnome::App::UI_ITEM, "Insert a _red book", nil, insert_red, nil,
        Gnome::App::PIXMAP_STOCK, Gnome::Stock::BOOK_RED, 0, 0, nil ],
      [ Gnome::App::UI_ITEM, "Insert a _green book", nil, insert_green, nil,
        Gnome::App::PIXMAP_STOCK, Gnome::Stock::BOOK_GREEN, 0, 0, nil ],
      [ Gnome::App::UI_ITEM, "Insert a _blue book", nil, insert_blue, nil,
        Gnome::App::PIXMAP_STOCK, Gnome::Stock::BOOK_BLUE, 0, 0, nil ],
    ]

    helper_shared_popup = [
      [ Gnome::App::UI_ITEM, "Insert a _red book", nil, insert_red, nil,
        Gnome::App::PIXMAP_STOCK, Gnome::Stock::BOOK_RED, 0, 0, nil ],
      [ Gnome::App::UI_ITEM, "Insert a _green book", nil, insert_green, nil,
        Gnome::App::PIXMAP_STOCK, Gnome::Stock::BOOK_GREEN, 0, 0, nil ],
      [ Gnome::App::UI_ITEM, "Insert a _blue book", nil, insert_blue, nil,
        Gnome::App::PIXMAP_STOCK, Gnome::Stock::BOOK_BLUE, 0, 0, nil ],
      Gnome::UIInfo::subtree("Subtree", helper_shared_popup_dup),
    ]

    # These function change the fill color of the canvas item specified in the user data
    set_cyan = Proc.new do |widget, item, for_widget|
      item.set({:fill_color => "cyan"})
    end

    set_magenta = Proc.new do |widget, item, for_widget|
      item.set({:fill_color => "magenta"})
    end

    set_yellow = Proc.new do |widget, item, for_widget|
      item.set({:fill_color => "yellow"})
    end

    
    # Explicit popup menu definition for the GnomeAppHelper test
    helper_explicit_popup_dup = [
      [ Gnome::App::UI_ITEM, "Set color to _cyan", nil, set_cyan, nil,
        Gnome::App::PIXMAP_NONE, nil, 0, 0, nil ],
      [ Gnome::App::UI_ITEM, "Set color to _magenta", nil, set_magenta, nil,
        Gnome::App::PIXMAP_NONE, nil, 0, 0, nil ],
      [ Gnome::App::UI_ITEM, "Set color to _yellow", nil, set_yellow, nil,
          Gnome::App::PIXMAP_NONE, nil, 0, 0, nil ],
    ]

    helper_explicit_popup = [
      [ Gnome::App::UI_ITEM, "Set color to _cyan", nil, set_cyan, nil,
        Gnome::App::PIXMAP_NONE, nil, 0, 0, nil ],
      [ Gnome::App::UI_ITEM, "Set color to _magenta", nil, set_magenta, nil,
        Gnome::App::PIXMAP_NONE, nil, 0, 0, nil ],
      [ Gnome::App::UI_ITEM, "Set color to _yellow", nil, set_yellow, nil,
        Gnome::App::PIXMAP_NONE, nil, 0, 0, nil ],
      Gnome::UIInfo::subtree("Subtree", helper_explicit_popup_dup),
    ]


    # start main

    helper_main_menu = self.create_menus(helper_main_menu)
    self.create_toolbar(helper_toolbar)

    bar = Gnome::AppBar.new(false, true, Gnome::PREFERENCES_USER)
    self.statusbar = bar

    bar.install_menu_hints(helper_main_menu)

    vbox = Gtk::VBox.new(false, Gnome::PAD_SMALL)
    vbox.border_width = Gnome::PAD_SMALL

    # Shared popup menu

    popup = Gnome::PopupMenu.new(helper_shared_popup)

    frame = Gtk::Frame.new("Shared popup menu")
    vbox.pack_start(frame, false, false, 0)
    frame.show()

    vbox2 = Gtk::VBox.new(false, Gnome::PAD_SMALL)
    vbox2.border_width = Gnome::PAD_SMALL
    frame.add(vbox2)
    vbox2.show()

    w = Gtk::Entry.new()
    vbox2.pack_start(w, false, false, 0)
    w.show()
    popup.attach(w, w)

    w = Gtk::Entry.new()
    vbox2.pack_start(w, false, false, 0)
    w.show()
    popup.attach(w, w)

    # Popup menu explicitly popped

    popup = Gnome::PopupMenu.new(helper_explicit_popup)

    frame = Gtk::Frame.new("Explicit popup menu")
    vbox.pack_start(frame, true, true, 0)
    frame.show()

    w = Gnome::Canvas.new()
    w.set_size_request(200, 100)
    w.set_scroll_region(0.0, 0.0, 200.0, 100.0)
    frame.add(w)
    w.show()

    w.signal_connect("destroy", popup) do |widget, popup|
      popup.destroy
    end

    item = Gnome::CanvasEllipse.new(w.root,
                                     { :x1 => 5.0,
                                       :y1 => 5.0,
                                       :x2 => 95.0,
                                       :y2 => 95.0,
                                       :fill_color => "white",
                                       :outline_color => "black"})
    item.signal_connect("event", popup) do |item, event, popup|
      item_event(item, event, popup)
    end

    item = Gnome::CanvasEllipse.new(w.root,
                                    { :x1 => 105.0,
                                      :y1 => 0.0,
                                      :x2 => 195.0,
                                      :y2 => 95.0,
                                      :fill_color => "white",
                                      :outline_color => "black"})
    item.signal_connect("event", popup) do |item, event, popup|
      item_event(item, event, popup)
    end

    self.contents = vbox
    self.show()
  end
end

if $0 == __FILE__
  Gnome::Program.new("testGNOME", TestGnomeApp::VERSION, Gnome::ModuleInfo::LIBGNOMEUI)
  app = AppHelperApp.new
  app.signal_connect("destroy") { Gtk::main_quit }
  Gtk::main
end

# Local variables:
# indent-tabs-mode: nil
# ruby-indent-level: 2
# End:
