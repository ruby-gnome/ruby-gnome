#!/usr/bin/env ruby

require 'gtk2'

label=Gtk::Label.new.show

info=Gtk::InfoBar.new
info.no_show_all=true
info.content_area.add(label)
info.add_button(Gtk::Stock::OK, Gtk::ResponseType::OK)
info.signal_connect('response'){|info, response| info.hide}

w=Gtk::Window.new
w.add(info)
w.show_all
w.signal_connect("delete_event"){Gtk.main_quit}

GLib::Timeout.add_seconds(1){
  label.text='ERROR'
  info.message_type=Gtk::MessageType::ERROR
  info.show
  false
}
Gtk.main
