#!/usr/bin/env ruby
=begin
  terminal.rb - Ruby/VTE sample script.

  Copyright (c) 2006 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: terminal.rb,v 1.2 2006/06/17 13:27:51 mutoh Exp $
=end

require "vte"

window = Gtk::Window.new("Terminal sample")
window.signal_connect("destroy"){Gtk.main_quit}

terminal = Vte::Terminal.new
terminal.set_font("Monospace 16", Vte::TerminalAntiAlias::FORCE_ENABLE)
terminal.signal_connect("child-exited") do |widget|
  Gtk.main_quit
end
terminal.signal_connect("window-title-changed") do |widget|
  window.title = terminal.window_title
end
terminal.fork_command
window.add(terminal)
window.show_all

Gtk.main
