#!/usr/bin/env ruby
# encoding: UTF-8

require 'gtk2'
require 'gst'

def usage
  $stderr.puts "Usage: #{$0} uri"
  exit 1
end

usage unless ARGV.length==1

playbin=Gst::ElementFactory.make('playbin')
playbin.ready
playbin.bus.add_watch {|bus, message|
  case message
  when Gst::MessageError
    $stderr.puts "ERROR: "+message.parse.to_s
  when Gst::MissingMessage
    puts "Installer detail: "+message.installer_detail.to_s
    if Gst::InstallPlugins.supported?
      context=Gst::InstallPluginsContext.new
      Gst::InstallPlugins.async([message.installer_detail], context){|ret| p ret}
    else
      $stderr.puts "Plugins installation is not supported."
    end
  end
  true
}
playbin.uri=ARGV[0]
playbin.play

label=Gtk::Label.new(ARGV[0])
window=Gtk::Window.new
window.add(label)
window.show_all
window.signal_connect('delete-event'){playbin.stop; Gtk.main_quit}

Gtk.main
