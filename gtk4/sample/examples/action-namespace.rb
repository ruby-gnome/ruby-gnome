#!/usr/bin/env ruby
#
# Copyright (C) 2018  Ruby-GNOME2 Project Team
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

# example from https://github.com/GNOME/gtk/blob/master/examples/action-namespace.c

require_relative "utils"

require_gtk4

MENU_UI =
"<interface>
  <menu id='doc-menu'>
    <section>
      <item>
        <attribute name='label'>_Save</attribute>
        <attribute name='action'>save</attribute>
      </item>
      <item>
        <attribute name='label'>_Print</attribute>
        <attribute name='action'>print</attribute>
      </item>
      <item>
        <attribute name='label'>_Share</attribute>
        <attribute name='action'>share</attribute>
      </item>
    </section>
  </menu>
  <menu id='win-menu'>
    <section>
      <item>
        <attribute name='label'>_Fullscreen</attribute>
        <attribute name='action'>fullscreen</attribute>
      </item>
      <item>
        <attribute name='label'>_Close</attribute>
        <attribute name='action'>close</attribute>
      </item>
    </section>
  </menu>
</interface>"

def action(name, window)
  action = Gio::SimpleAction.new(name)
  action.signal_connect "activate" do |_simple_action, _parameter|
    dialog = Gtk::MessageDialog.new(:parent => window,
                                    :flags => :destroy_with_parent,
                                    :buttons => :close,
                                    :message => "Action #{name} activated.")
    dialog.signal_connect(:response) do
      dialog.destroy
    end
    dialog.show
  end
  action
end

def action_save(window)
  action("save", window)
end

def action_print(window)
  action("print", window)
end

def action_share(window)
  action("share", window)
end

def action_fullscreen(window)
  action("fullscreen", window)
end

def action_close(window)
  action("close", window)
end

application = Gtk::Application.new("org.gtk.example", :flags_none)

application.signal_connect "activate" do |app|
  win = Gtk::ApplicationWindow.new(app)
  win.set_default_size(200, 300)

  doc_actions = Gio::SimpleActionGroup.new
  doc_actions.add_action(action_save(win))
  doc_actions.add_action(action_share(win))
  doc_actions.add_action(action_print(win))

  win.add_action(action_close(win))
  win.add_action(action_fullscreen(win))

  builder = Gtk::Builder.new(:string => MENU_UI)
  docmenu = builder["doc-menu"]
  winmenu = builder["win-menu"]

  buttonmenu = Gio::Menu.new

  section = Gio::MenuItem.new(nil, docmenu)
  section.action_namespace = "doc"
  buttonmenu.append_item(section)

  section = Gio::MenuItem.new(nil, winmenu)
  section.action_namespace = "win"
  buttonmenu.append_item(section)

  button = Gtk::MenuButton.new
  button.label = "Menu"
  button.insert_action_group("doc", doc_actions)
  button.menu_model = buttonmenu
  button.halign = :center
  button.valign = :start
  win.add(button)
  win.show
end

application.run(ARGV)
