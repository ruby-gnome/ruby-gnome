#!/usr/bin/env ruby
=begin
  aboutdialog.rb - Ruby/GTK sample script.

  Copyright (c) 2005-2020 Ruby-GNOME Project Team
  This program is licenced under the same licence as Ruby-GNOME.
=end

require "gtk3"

unless Gtk::Version.or_later?(3, 4, 2)
  puts "This sample requires GTK+ 3.4.2 or later: #{Gtk::Version::STRING}"
  exit
end

a = Gtk::AboutDialog.new
a.artists            = ["Artist 1 <no1@foo.bar.com>", "Artist 2 <no2@foo.bar.com>"]
a.authors            = ["Author 1 <no1@foo.bar.com>", "Author 2 <no2@foo.bar.com>"]
a.comments           = "This is a sample script for Gtk::AboutDialog"
a.copyright          = "Copyright (C) 2020 Ruby-GNOME Project"
a.documenters        = ["Documenter 1 <no1@foo.bar.com>", "Documenter 2 <no2@foo.bar.com>"]
a.license            = "This program is licenced under the same licence as Ruby-GNOME."
a.logo               = GdkPixbuf::Pixbuf.new(:file => File.join(__dir__, "gnome-logo-icon.png"))
a.program_name       = "Gtk::AboutDialog sample"
a.translator_credits = "Translator 1\nTranslator 2\n"
a.version            = "1.0.0"
a.website            = "https://ruby-gnome2.osdn.jp"
a.website_label      = "Ruby-GNOME Project Website"

a.signal_connect("activate-link") do |_widget, uri|
  p _widget.class
  p uri
end

p a.run
