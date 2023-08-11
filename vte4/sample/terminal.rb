#!/usr/bin/env ruby
=begin
  terminal.rb - Ruby/VTE4 sample script.

  Copyright (c) 2006-2023  Ruby-GNOME Project Team
  This program is licenced under the same licence as Ruby-GNOME.
=end

require "vte4"

application = Gtk::Application.new("io.github.ruby-gnome.example", :flags_none)

application.signal_connect "activate" do |app|
  window = Gtk::ApplicationWindow.new(app)
  window.title = "Terminal sample"

  terminal = Vte::Terminal.new
  terminal.enable_sixel = true
  terminal.signal_connect("child-exited") do |_widget|
    window.destroy
  end
  terminal.signal_connect("window-title-changed") do |_widget|
    window.title = terminal.window_title
  end
  terminal.spawn
  window.child = terminal

  window.present
end

application.run
