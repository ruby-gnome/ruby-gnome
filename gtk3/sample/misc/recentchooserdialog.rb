#!/usr/bin/env ruby
=begin
  recentchooserdialog.rb - Ruby/GTK2 sample script.

  Copyright (c) 2006 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: recentchooserdialog.rb,v 1.2 2006/11/15 23:46:13 mutoh Exp $
=end

require 'gtk2'

if str = Gtk.check_version(2, 10, 0)
  puts "This sample requires GTK+ 2.10.0 or later"
  puts str
  exit
end

dialog = Gtk::RecentChooserDialog.new("Recent Chooser Dialog Sample",
                                      nil,
                                      [Gtk::Stock::CANCEL, Gtk::Dialog::RESPONSE_CANCEL], 
                                      [Gtk::Stock::OPEN, Gtk::Dialog::RESPONSE_ACCEPT])

dialog.signal_connect("response") do |widget, response|
  case response
  when Gtk::Dialog::RESPONSE_ACCEPT
    info = dialog.current_item
    if info
      puts "----"
      puts info.uri
      puts info.display_name
      puts info.description
      puts info.mime_type
      puts info.added
      puts info.modified
      puts info.visited
      puts info.private_hint?
      p    info.application_info("gedit")
      p    info.applications
      puts info.last_application
      p    info.groups
      puts info.has_group?("web")
      puts info.has_application?("gedit")
      puts info.get_icon(64)
      puts info.short_name
      puts info.uri_display
      puts info.age
      puts info.local?
      puts info.exist?
    end
  else
    p "Close"
    dialog.destroy
    Gtk.main_quit
  end
end
dialog.show_all

Gtk.main
