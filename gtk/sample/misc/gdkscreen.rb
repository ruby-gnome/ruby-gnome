=begin
  setting.rb - Gtk::Settings sample script.

  Copyright (C) 2004 Masao Mutoh <mutoh@highway.ne.jp>
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: gdkscreen.rb,v 1.1 2004/03/24 17:54:29 mutoh Exp $
=end

require 'gtk2'

Gtk.init

screen = Gdk::Screen.default
settings_map = [
  "gtk-double-click-time",
  "gtk-dnd-drag-threshold",
  "gtk-can-change-accels",
  "gtk-color-palette",
  "gtk-font-name",
  "gtk-icon-sizes",
  "gtk-key-theme-name",
  "gtk-toolbar-style",
  "gtk-toolbar-icon-size",
  "gtk-im-preedit-style",
  "gtk-im-status-style",
  "gtk-cursor-blink",
  "gtk-cursor-blink-time",
  "gtk-theme-name" 
]

settings_map.each do |v|
  puts "#{v}: #{screen.get_setting(v)}"
end
