#!/usr/local/bin/ruby
=begin
  format.rb - Ruby/GdkPixbuf sample script.

  Copyright (c) 2004 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: format.rb,v 1.1 2004/08/27 20:29:28 mutoh Exp $
=end

require 'gdk_pixbuf2'

filename = ARGV[0]
unless filename
  puts "ruby #{$0} filename"
  exit(1)
end

p fileinfo = Gdk::Pixbuf.get_file_info(filename)
p fileinfo.name
p fileinfo.description
p fileinfo.mime_types
p fileinfo.extensions
p fileinfo.writable?
p fileinfo.domain
p fileinfo.signature

