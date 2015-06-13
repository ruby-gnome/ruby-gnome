#!/usr/bin/env ruby
=begin
  recentchooserdialog.rb - Ruby/GTK sample script.

  Copyright (c) 2006-2015 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.
=end

require "gtk3"

dialog = Gtk::RecentChooserDialog.new(:title => "Recent Chooser Dialog Sample",
                                      :buttons => [["_Cancel", Gtk::ResponseType::CANCEL],
                                                   ["_Accept", Gtk::ResponseType::ACCEPT]])

dialog.signal_connect("response") do |_widget, response|
  case response
  when Gtk::ResponseType::ACCEPT
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
      p    info.get_application_info("gedit")
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
