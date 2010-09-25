#!/usr/bin/env ruby
=begin
  aboutdialog.rb - Ruby/GTK sample script.

  Copyright (c) 2005,2006 Ruby-GNOME2 Project Team 
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: aboutdialog.rb,v 1.2 2006/06/17 13:18:12 mutoh Exp $
=end
require 'gtk2'

if str = Gtk.check_version(2, 6, 0)
  puts "This sample requires GTK+ 2.6.0 or later"
  puts str
  exit
end

Gtk::AboutDialog.set_email_hook {|about, link|
  p "email_hook"
  p link
}
Gtk::AboutDialog.set_url_hook {|about, link|
  p "url_hook"
  p link
}

a = Gtk::AboutDialog.new
a.artists   = ["Artist 1 <no1@foo.bar.com>", "Artist 2 <no2@foo.bar.com>"]
a.authors = ["Author 1 <no1@foo.bar.com>", "Author 2 <no2@foo.bar.com>"]
a.comments  = "This is a sample script for Gtk::AboutDialog"
a.copyright = "Copyright (C) 2005 Ruby-GNOME2 Project"
a.documenters = ["Documenter 1 <no1@foo.bar.com>", "Documenter 2 <no2@foo.bar.com>"]
a.license   = "This program is licenced under the same licence as Ruby-GNOME2."
a.logo      = Gdk::Pixbuf.new("gnome-logo-icon.png")
a.program_name = "Gtk::AboutDialog sample"
a.translator_credits = "Translator 1\nTranslator 2\n"
a.version   = "1.0.0"
a.website   = "http://ruby-gnome2.sourceforge.jp"
a.website_label = "Ruby-GNOME2 Project Website"

p a.run

