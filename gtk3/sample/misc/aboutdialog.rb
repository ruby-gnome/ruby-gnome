#!/usr/bin/env ruby
=begin
  aboutdialog.rb - Ruby/GTK sample script.

  Copyright (c) 2005-2015 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: aboutdialog.rb,v 1.2 2006/06/17 13:18:12 mutoh Exp $
=end
require 'gtk3'

unless Gtk::Version.or_later?(3, 4, 2)
  puts "This sample requires GTK+ 3.4.2 or later: #{Gtk::Version::STRING}"
  exit
end

a = Gtk::AboutDialog.new
a.artists   = ["Artist 1 <no1@foo.bar.com>", "Artist 2 <no2@foo.bar.com>"]
a.authors = ["Author 1 <no1@foo.bar.com>", "Author 2 <no2@foo.bar.com>"]
a.comments  = "This is a sample script for Gtk::AboutDialog"
a.copyright = "Copyright (C) 2005 Ruby-GNOME2 Project"
a.documenters = ["Documenter 1 <no1@foo.bar.com>", "Documenter 2 <no2@foo.bar.com>"]
a.license   = "This program is licenced under the same licence as Ruby-GNOME2."
a.logo      = Gdk::Pixbuf.new(File.join(__dir__, "gnome-logo-icon.png"))
a.program_name = "Gtk::AboutDialog sample"
a.translator_credits = "Translator 1\nTranslator 2\n"
a.version   = "1.0.0"
a.website   = "http://ruby-gnome2.sourceforge.jp"
a.website_label = "Ruby-GNOME2 Project Website"

a.signal_connect 'activate-link' do |about_dialog, uri|
  p widget.class
  p uri
end

response = a.run
case
when Gtk::ResponseType::DELETE_EVENT == response
  puts "Gtk::ResponseType::DELETE_EVENT" # Gtk 3.14.8
when Gtk::ResponseType::CANCEL == response
  puts "Gtk::ResponseType::CANCEL" # v3.4.2 >= Gtk <= v3.8.8
else
  puts "Another Response Type"
end
