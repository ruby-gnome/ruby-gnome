#!/usr/bin/env ruby

if ARGV.size != 1
  $stderr.puts "Usage: #{$0} uri"
  exit 1
end

require 'gtk2'
require 'gst'
require 'uri'

def normalize_uri(uri)
  uri = URI(uri)
  uri.scheme ||= "file"
  if uri.scheme == "file"
    "file://#{uri.path}"
  else
    uri.to_s
  end
end

play_bin = Gst::ElementFactory.make('playbin')
bus = play_bin.bus
bus.add_watch do |bus, message|
  case message.type
  when Gst::Message::EOS
    puts "end of stream"
    Gtk.main_quit
  when Gst::Message::ERROR
    $stderr.puts "Error: #{message.parse_error} (#{message.parse_info})"
    Gtk.main_quit
  end
  true
end

play_bin.uri = normalize_uri(ARGV.first)
video_sink = Gst::ElementFactory.make('xvimagesink')
play_bin.video_sink = video_sink

window = Gtk::Window.new
window.signal_connect('destroy') {Gtk.main_quit}
window.signal_connect('expose-event') do
  video_sink.xwindow_id = window.window.xid
end
window.show_all

puts "Streaming #{play_bin.uri}..."
play_bin.play

begin
  Gtk.main
rescue Interrupt
  window.destroy
end
