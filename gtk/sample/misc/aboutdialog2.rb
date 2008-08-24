#!/usr/bin/env ruby
=begin
  aboutdialog2.rb - Ruby/GTK sample script.

  Copyright (c) 2005,2006 Ruby-GNOME2 Project Team 
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: aboutdialog2.rb,v 1.2 2006/06/17 13:18:12 mutoh Exp $
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

Gtk::AboutDialog.show(nil,
		      "artists" => ["Artist 1 <no1@foo.bar.com>", "Artist 2 <no2@foo.bar.com>"],
		      "authors" => ["Author 1 <no1@foo.bar.com>", "Author 2 <no2@foo.bar.com>"],
		      "comments" => "This is a sample script for Gtk::AboutDialog",
		      "copyright" => "Copyright (C) 2005 Ruby-GNOME2 Project",
		      "documenters" => ["Documenter 1 <no1@foo.bar.com>", "Documenter 2 <no2@foo.bar.com>"],
		      "license" => "This program is licenced under the same licence as Ruby-GNOME2.",
		      "logo_icon_name" => "gtk-home",
		      "program_name" => "Gtk::AboutDialog sample",
		      "translator_credits" => "Translator 1 <no1@foo.bar.com>\nTranslator 2 <no2@foo.bar.com>\n",
		      "version" => "1.0.0",
		      "website" => "http://ruby-gnome2.sourceforge.jp",
		      "website_label" => "Ruby-GNOME2 Project Website"
		      )

Gtk.main
