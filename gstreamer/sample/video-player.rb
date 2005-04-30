# Video streaming via Gst::Playbin.

require 'gtk2'
require 'gst'

Gtk.init
Gst.init

if ARGV.length != 1
    $stderr.puts "Usage: #{__FILE__} uri"
    exit 1
end

playbin = Gst::ElementFactory.make('playbin')
playbin.signal_connect('eos') do 
    puts "end of stream"; 
    Gtk.main_quit
end
playbin.signal_connect('error') do |element, source, error, debug|
    $stderr.puts "Error: #{error} (#{debug})"
    Gtk.main_quit
end
playbin.uri = ARGV.first
videosink = Gst::ElementFactory.make('xvimagesink')
playbin.video_sink = videosink

window = Gtk::Window.new
window.signal_connect('destroy') { Gtk.main_quit }
window.signal_connect('expose-event') { videosink.xwindow_id = window.window.xid  }
window.show_all

puts "Streaming #{playbin.uri}..."
playbin.play

Gtk.main
