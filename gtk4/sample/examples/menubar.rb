#!/usr/bin/env ruby
#
# Copyright (C) 2023  ToshioCP (Toshio Sekiya), Ruby-GNOME2 Project Team
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

# Ruby implementation of the C example in:
# https://toshiocp.github.io/Gtk4-tutorial/sec17.html

require_relative "utils"

require_gtk4

app = Gtk::Application.new("org.gtk.example", :flags_none)

app.signal_connect "activate" do
  window = Gtk::ApplicationWindow.new(app)
  window.application = app

  act_quit = Gio::SimpleAction.new("quit")
  app.add_action(act_quit)
  act_quit.signal_connect "activate" do |_simple_action, _parameter|
    window.destroy
  end

  menubar = Gio::Menu.new
  menu_item_menu = Gio::MenuItem.new("Menu")
  menu = Gio::Menu.new
  menu_item_quit = Gio::MenuItem.new("Quit", "app.quit")
  menu.append_item(menu_item_quit)
  menu_item_menu.submenu = menu
  menubar.append_item(menu_item_menu)
  app.menubar = menubar
  window.show_menubar = true

  window.show
end

app.run
