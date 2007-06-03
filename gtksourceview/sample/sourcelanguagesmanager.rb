#!/usr/bin/env ruby
=begin
  sourcelanguagesmanager.rb - Ruby/GtkSourceView sample script.

  Copyright (c) 2006 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: sourcelanguagesmanager.rb,v 1.3 2007/06/03 02:11:07 mutoh Exp $
=end

require 'gtksourceview'

s = Gtk::SourceLanguagesManager.new
s.available_languages.each do |v|
  puts v.name
end
s.lang_files_dirs.each do |v|
  puts v
end
puts s.get_language("text/html").name

