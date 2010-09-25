#!/usr/bin/env ruby
=begin
  linkbutton.rb - Gtk::LinkButton sample.

  Copyright (c) 2006 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: linkbutton.rb,v 1.1 2006/10/21 16:58:00 mutoh Exp $
=end

require 'gtk2'

window = Gtk::Window.new("Gtk::LinkButton sample")
window.signal_connect("destroy"){Gtk.main_quit}

vbox = Gtk::VBox.new

# URI only
button1 = Gtk::LinkButton.new("http://ruby-gnome2.sourceforge.jp/")
button1.signal_connect("clicked") do
  puts button1.uri
end
vbox.pack_start(button1)

# URI with a label
button2 = Gtk::LinkButton.new("http://ruby-gnome2.sourceforge.jp/",
			      "Ruby-GNOME2 Website")
button2.signal_connect("clicked") do
  puts button2.uri
end

# Global setting instead of using clicked signals.
Gtk::LinkButton.set_uri_hook {|button, link|
  puts "set_uri_hook: " + link
}

vbox.pack_start(button2)

window.add(vbox)
window.show_all

Gtk.main
