=begin
  uimanager2.rb - 

  Copyright (C) 2004 Masao Mutoh<mutoh@highway.ne.jp>

  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: uimanager2.rb,v 1.2 2004/07/31 05:44:45 mutoh Exp $
=end
require 'gtk2'

Gtk.init

if str = Gtk.check_version(2, 4, 0)
  puts "This sample requires GTK+ 2.4.0 or later"
  puts str
  exit
end

callback = Proc.new {|actiongroup, action| 
  puts "`#{action.name}' is clicked. "
  if action.is_a? Gtk::ToggleAction
    puts "active? = #{action.active?}"
  end
}

callback_quit = Proc.new {
  p "Quit is called."
  Gtk.main_quit
}

callback_radio = Proc.new {|action, current|
  puts "action = `#{action.name}'"
  puts "current = `#{current.name}'"
}

actions = [
  ["FileMenu", nil, "_File"],
  ["PreferencesMenu", nil, "_Preferences"],
  ["ColorMenu", nil, "_Color"],
  ["ShapeMenu", nil, "_Shape"],
  ["HelpMenu", nil, "_Help"],
  ["New", Gtk::Stock::NEW, "_New", "<control>N", "Create a new file", callback],
  ["Open", Gtk::Stock::OPEN, "_Open", "<control>O", "Open a file", callback],
  ["Save", Gtk::Stock::SAVE, "_Save", "<control>S", "Save current file", callback],
  ["SaveAs", Gtk::Stock::SAVE, "Save _As...", nil, "Save to a file", callback],
  ["Quit", Gtk::Stock::QUIT, "_Quit", "<control>Q", "Quit", callback_quit],
  ["About", nil, "_About", "<control>A", "About", callback],
  ["Logo", "demo-gtk-logo", nil, nil, "GTK+", callback]
]

toggle_actions = [
  ["Bold", Gtk::Stock::BOLD, "_Bold", "<control>B", "Bold", callback, true]
]

color_radio_actions = [
  ["Red", nil, "_Red", "<control>R", "Blood", 0],
  ["Green", nil, "_Green", "<control>G", "Grass", 1],
  ["Blue", nil, "_Blue", "<control>B", "Sky", 2]
]

shape_radio_actions = [
  ["Square", nil, "_Square", "<control>S", "Square", 0],
  ["Rectangle", nil, "_Rectangle", "<control>R", "Rectangle", 1],
  ["Oval", nil, "_Oval", "<control>O", "Egg", 2]
]

window = Gtk::Window.new

actiongroup = Gtk::ActionGroup.new("Actions")
actiongroup.add_actions(actions)
actiongroup.add_toggle_actions(toggle_actions)
actiongroup.add_radio_actions(color_radio_actions, 1) do |action, current|
  puts "action = `#{action.name}'"
  puts "current = `#{current.name}'"
end
actiongroup.add_radio_actions(shape_radio_actions, 2, callback_radio)

uimanager = Gtk::UIManager.new
uimanager.insert_action_group(actiongroup, 0)
window.add_accel_group(uimanager.accel_group)

uimanager.add_ui("./uimanager2.xml")

vbox = Gtk::VBox.new
vbox.pack_start(uimanager.get_widget("/MenuBar"), false, false)
vbox.pack_start(Gtk::Label.new("Gtk::UIManager Sample"))
window.add(vbox)
window.set_default_size(100, 100).show_all
Gtk.main

