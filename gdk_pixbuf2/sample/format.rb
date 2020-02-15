#!/usr/bin/env ruby
=begin
  format.rb - Ruby/GdkPixbuf sample script.

  Copyright (c) 2004-2020 Ruby-GNOME Project Team
  This program is licenced under the same licence as Ruby-GNOME.

  $Id: format.rb,v 1.4 2006/06/17 14:38:08 mutoh Exp $
=end

require 'gtk3'

filename = ARGV[0]
unless filename
  puts "ruby #{$0} filename"
  exit(1)
end

puts fileinfo = GdkPixbuf::Pixbuf.get_file_info(filename)[0]
puts "name        = #{fileinfo.name}"
puts "description = #{fileinfo.description}"
puts "mime_types  = #{fileinfo.mime_types.inspect}"
puts "extensions  = #{fileinfo.extensions.inspect}"
puts "writable    = #{fileinfo.writable?}"
puts "scalable    = #{fileinfo.scalable?}"
puts "disabled    = #{fileinfo.disabled?}"
puts "license     = #{fileinfo.license.inspect}"
