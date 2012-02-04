#!/usr/bin/env ruby
# coding: utf-8

unless file = ARGV.shift
  puts "Usage: #{$0} <media file>"
  exit(false)
end

unless %w(gtk3 gtk2).find{|l| begin; require l; rescue LoadError; false; end}
  puts "This sample requires gtk3 or gtk2."
  exit(false)
end
require 'gvlc'

player = VLC::MediaPlayer.new(path: file)

window = Gtk::Window.new
view = Gtk::DrawingArea.new
window.add(view)

window.signal_connect(:destroy){Gtk.main_quit}
view.signal_connect(:realize) do |widget|
  player.xwindow = widget.window.xid
end
player.attach_event(:mediaplayer_playing) do |player, event|
  size = player.get_size(0)
  view.set_size_request(*size) if size
end

window.show_all
player.play
Gtk.main

