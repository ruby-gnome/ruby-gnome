#!/usr/bin/env ruby
=begin
  terminal.rb - Ruby/VTE3 sample script.

  Copyright (c) 2006-2020 Ruby-GNOME Project Team
  This program is licenced under the same licence as Ruby-GNOME.

  $Id: terminal.rb,v 1.2 2006/06/17 13:27:51 mutoh Exp $
=end

require "vte3"

window = Gtk::Window.new("Terminal sample")
window.signal_connect("destroy"){ Gtk.main_quit }

terminal = Vte::Terminal.new
terminal.enable_sixel = true if terminal.respond_to?(:enable_sixel=)
terminal.signal_connect("child-exited") do |_widget|
  Gtk.main_quit
end
terminal.signal_connect("window-title-changed") do |_widget|
  window.title = terminal.window_title
end
terminal.spawn
window.add(terminal)
window.show_all

Gtk.main
