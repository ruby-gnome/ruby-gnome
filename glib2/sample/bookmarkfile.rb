=begin
  bookmarkfile.rb - Sample for GLib::BookmarkFile

  Copyright (C) 2006 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: bookmarkfile.rb,v 1.1 2006/12/26 09:59:51 mutoh Exp $
=end

require 'glib2'

$KCODE = "U"

#
# Create bookmarkfile data.
#
URI = "https://ruby-gnome2.osdn.jp/"
bf = GLib::BookmarkFile.new
bf.set_title(URI, "Ruby-GNOME2 sample")
bf.set_description(URI, "Ruby-GNOME2 Sampe for GLib::BookmarkFile")
bf.set_mime_type(URI, "text/html")
bf.set_private(URI, false)
bf.set_icon(URI, "https://ruby-gnome2.osdn.jp/logo-gy.png", "image/png")
bf.set_added(URI, Time.now)
bf.set_modified(URI, Time.now)
bf.set_visited(URI, Time.now)
bf.set_groups(URI, ["Ruby", "GTK+"])
bf.set_app_info(URI, "WWW Browser", "firefox %u", 1, Time.now)
bf.add_group(URI, "GNOME")
bf.add_application(URI, "Ruby VM", "ruby %u")

#bf.remove_group(URI, "GTK+")
#bf.remove_application(URI, "Ruby VM")
#bf.remove_item(URI)
#bf.move_item(URI, "http://gtk.org/")

# Save as "bookmarkfile.xml"
bf.to_file("bookmarkfile.xml")

#
# Load from "bookmarkfile.xml"
#
bf2 = GLib::BookmarkFile.new
bf2.load_from_file("bookmarkfile.xml")

puts "size = #{bf2.size}"
puts "uris = #{bf2.uris.inspect}"
bf2.uris.each do |uri|
  puts "uri: [#{uri}]"
  puts "  * title: [#{bf2.get_title(uri)}]"
  puts "  * description: [#{bf2.get_description(uri)}]"
  puts "  * mime_type: [#{bf2.get_mime_type(uri)}]"
  puts "  * private?: [#{bf2.private?(uri)}]"
  puts "  * icon: [#{bf2.get_icon(uri).inspect}]"
  puts "  * added: [#{bf2.get_added(uri)}]"
  puts "  * modified: [#{bf2.get_modified(uri)}]"
  puts "  * visited: [#{bf2.get_visited(uri)}]"
  puts "  * groups: #{bf2.get_groups(uri).inspect}"
  puts "  * applications: #{bf2.get_applications(uri).inspect}"
  begin
    puts "  * app_info: #{bf2.get_app_info(uri, "WWW Browser").inspect}"
  rescue
    puts $!
  end
  puts
end
