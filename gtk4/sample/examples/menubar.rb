#!/usr/bin/env ruby
#
# Copyright (C) 2023  Ruby-GNOME2 Project Team
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

# example from https://toshiocp.github.io/Gtk4-tutorial/sec17.html

require_relative "utils"

require_gtk4

app = Gtk::Application.new("org.gtk.example", :flags_none)

app.signal_connect "activate" do
  window = Gtk::ApplicationWindow.new(app)
  window.set_application(app)

  act_quit = Gio::SimpleAction.new("quit")
  app.add_action(act_quit)
  act_quit.signal_connect "activate" do |_simple_action, _parameter|
    window.destroy
    exit!
  end

  menubar = Gio::Menu.new
  menu_item_menu = Gio::MenuItem.new("Menu", nil)
  menu = Gio::Menu.new
  menu_item_quit = Gio::MenuItem.new("Quit", "app.quit")
  menu.append_item(menu_item_quit)
  menu_item_menu.set_submenu(menu)
  menubar.append_item(menu_item_menu)
  app.menubar = menubar
  window.set_show_menubar true

  window.show
end

app.run
