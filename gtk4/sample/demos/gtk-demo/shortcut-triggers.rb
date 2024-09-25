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
#   https://gitlab.gnome.org/GNOME/gtk/-/blob/main/demos/gtk-demo/shortcut_triggers.c
#
# See the following for license information:
#   * https://gitlab.gnome.org/GNOME/gtk/-/blob/main/AUTHORS
#   * https://gitlab.gnome.org/GNOME/gtk/-/blob/main/COPYING

class ShortcutTriggersDemo < GtkDemo
  def initialize
    super(__FILE__,
          nil,
          "Shortcuts",
          <<-DESCRIPTION,
Gtk::Shortcut is the abstraction used by GTK to handle shortcuts from
keyboard or other input devices.

Shortcut triggers can be used to weave complex sequences of key
presses into sophisticated mechanisms to activate shortcuts.

This demo code shows creative ways to do that.
          DESCRIPTION
          ["Gtk::ShortcutController"])
  end

  def run(parent)
    window = Gtk::Window.new
    window.display = parent.display
    window.title = "Shortcuts"
    window.set_default_size(200, -1)
    window.resizable = false

    list = Gtk::ListBox.new
    list.margin_top = 6
    list.margin_bottom = 6
    list.margin_start = 6
    list.margin_end = 6
    window.child = list

    [
      ["Press Ctrl-g", Gtk::KeyvalTrigger.new(Gdk::Keyval::KEY_g, :control_mask)],
      ["Press x", Gtk::KeyvalTrigger.new(Gdk::Keyval::KEY_x, 0)],
    ].each do |label, trigger|
      row = Gtk::Label.new(label)
      list.append(row)
      controller = Gtk::ShortcutController.new
      controller.scope = :global
      row.add_controller(controller)

      action = Gtk::CallbackAction.new do
        puts("activated: #{row.label}\n")
      end
      shortcut = Gtk::Shortcut.new(trigger, action)
      controller.add_shortcut(shortcut)
    end

    window.present

    window
  end

  GtkDemo.register(new)
end
