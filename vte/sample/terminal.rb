#!/usr/bin/env ruby

require "vte"

Gtk.init

window = Gtk::Window.new
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
