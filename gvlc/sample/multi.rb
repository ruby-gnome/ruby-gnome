#!/usr/bin/env ruby
# coding: utf-8

if ARGV.empty?
  puts "Usage: #{$0} <media file> ..."
  exit(false)
end

unless %w(gtk3 gtk2).find{|l| begin; require l; rescue LoadError; false; end}
  puts "This sample requires gtk3 or gtk2."
  exit(false)
end
require 'gvlc'

def add_player(box, file)
  player = VLC::MediaPlayer.new(path: file)
  view = Gtk::DrawingArea.new
  box.pack_start(view)

  view.signal_connect(:realize) do |widget|
    player.xwindow = widget.window.xid
  end
  player.attach_event(:mediaplayer_playing) do |player, event|
    size = player.get_size(0)
    view.set_size_request(*size) if size
  end

  player
end

window = Gtk::Window.new
box = Gtk::HBox.new
window.add(box)
players = ARGV.map{|f| add_player(box, f)}

window.signal_connect(:destroy){Gtk.main_quit}

window.show_all
players.each(&:play)
Gtk.main

