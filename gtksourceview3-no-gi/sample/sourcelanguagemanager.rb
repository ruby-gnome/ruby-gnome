#!/usr/bin/env ruby
=begin
  sourcelanguagesmanager.rb - Ruby/GtkSourceView3 sample script.

  Copyright (c) 2006-2014  Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: sourcelanguagesmanager.rb,v 1.3 2007/06/03 02:11:07 mutoh Exp $
=end

require "gtksourceview3"

s = GtkSource::LanguageManager.new
s.language_ids.each do |v|
  puts v
end
s.search_path.each do |v|
  puts v
end
puts s.get_language("html").name

