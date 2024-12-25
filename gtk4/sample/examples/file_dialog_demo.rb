# Copyright (C) 2024  Ruby-GNOME Project Team
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

# Original:
#   https://gitlab.gnome.org/GNOME/gtk/-/blob/main/demos/gtk-demo/main.c
#
# See the following for license information:
#   * https://gitlab.gnome.org/GNOME/gtk/-/blob/main/AUTHORS
#   * https://gitlab.gnome.org/GNOME/gtk/-/blob/main/COPYING

# We need to use "GtkDemo" as type name because "menu-listitem.ui"
# uses "GtkDemo" as type name.

require "gtk4"

def on_activate(app)
  win = Gtk::ApplicationWindow.new(app)
  dialog = Gtk::FileDialog.new
  dialog.modal = true
  dialog.set_initial_file(Gio::File.new_for_path(File.expand_path(__FILE__))) 
  dialog.set_initial_folder(Gio::File.new_for_path(Dir.pwd))
  dialog.save do |_dialog, result|
    begin
      file = dialog.save_finish(result)
      puts("file.path: #{file.path}")
    rescue
      puts("Cancel pressed")
    end
    win.destroy
  end
end

app = Gtk::Application.new("io.github.ruby-gnome.gtk4.example.FileDialog")
app.signal_connect("activate") do
  on_activate(app)
end
app.run
