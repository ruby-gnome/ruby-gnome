#!/usr/bin/env ruby
=begin
  toolbar.rb - Ruby/GTK3 sample script.

  Copyright (c) 2002-2015 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
=end

require "gtk3"

test_xpm_file = "#{File.expand_path(File.dirname(__FILE__))}/test.xpm"
window = Gtk::Window.new("Gtk::Toolbar sample")

window.signal_connect("destroy") do
  Gtk.main_quit
end
window.set_default_size 600, 600

toolbar = Gtk::Toolbar.new

button_horizontal = Gtk::ToolButton.new(:icon_widget => Gtk::Image.new(:file => test_xpm_file),
                                        :label => "Horizontal toolbar layout")
button_horizontal.tooltip_text = "Toolbar/Horizontal"
button_horizontal.signal_connect "clicked" do 
  toolbar.orientation = :horizontal
end

button_vertical = Gtk::ToolButton.new(:icon_widget => Gtk::Image.new(:file => test_xpm_file),
                                      :label => "Vertical toolbar layout")
button_vertical.tooltip_text = "Toolbar/Vertical"
button_vertical.signal_connect "clicked" do 
  toolbar.orientation = :vertical
end

button_icons_only = Gtk::ToolButton.new(:icon_widget => Gtk::Image.new(:file => test_xpm_file),
                                        :label => "Only show toolbar icons")
button_icons_only.tooltip_text = "Toolbar/IconsOnly"
button_icons_only.signal_connect "clicked" do
  toolbar.style = Gtk::ToolbarStyle::ICONS
end

button_text_only =  Gtk::ToolButton.new(:icon_widget => Gtk::Image.new(:file => test_xpm_file),
                                        :label => "Only show toolbar text")
button_text_only.tooltip_text = "Toolbar/TextOnly"
button_text_only.signal_connect "clicked" do
  toolbar.style = Gtk::ToolbarStyle::TEXT
end

button_both = Gtk::ToolButton.new(:icon_widget => Gtk::Image.new(:file => test_xpm_file),
                                  :label => "Show toolbar icons and text")
button_both.tooltip_text = "Toolbar/Both"
button_both.signal_connect "clicked" do 
  toolbar. style = Gtk::ToolbarStyle::BOTH
end

toolbar.insert(button_both, 0)
toolbar.insert(button_text_only, 0)
toolbar.insert(button_icons_only, 0)
toolbar.insert(button_vertical, 0)
toolbar.insert(button_horizontal, 0)
window.add(toolbar)
window.show_all

Gtk.main
