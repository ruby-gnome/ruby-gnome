#!/usr/bin/env ruby
=begin
  aboutdialog2.rb - Ruby/GTK sample script.

  Copyright (c) 2005-2020 Ruby-GNOME Project Team
  This program is licenced under the same licence as Ruby-GNOME.
=end
require "gtk3"

unless Gtk::Version.or_later?(3, 4, 2)
  puts "This sample requires GTK+ 3.4.2 or later: #{Gtk::Version::STRING}"
  exit
end

about_dialog = Gtk::AboutDialog.show(
  nil,
  "artists" => ["Artist 1 <no1@foo.bar.com>", "Artist 2 <no2@foo.bar.com>"],
  "authors" => ["Author 1 <no1@foo.bar.com>", "Author 2 <no2@foo.bar.com>"],
  "comments" => "This is a sample script for Gtk::AboutDialog",
  "copyright" => "Copyright (C) 2005-2020 Ruby-GNOME Project",
  "documenters" => ["Documenter 1 <no1@foo.bar.com>", "Documenter 2 <no2@foo.bar.com>"],
  "license" => "This program is licenced under the same licence as Ruby-GNOME.",
  "logo_icon_name" => "gtk-home",
  "program_name" => "Gtk::AboutDialog sample",
  "translator_credits" => "Translator 1 <no1@foo.bar.com>\nTranslator 2 <no2@foo.bar.com>\n",
  "version" => "1.0.0",
  "website" => "https://ruby-gnome2.osdn.jp",
  "website_label" => "Ruby-GNOME Project Website"
)

about_dialog.signal_connect('delete_event') do
  Gtk.main_quit
end

Gtk.main
