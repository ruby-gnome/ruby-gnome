=begin
  spawn.rb - Sample for GLib::Spawn

  Copyright (C) 2005 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Date: 2005/03/05 13:16:44 $
  $Id: spawn.rb,v 1.1 2005/03/05 13:16:44 mutoh Exp $
=end

require 'glib2'

p GLib::Spawn.command_line_sync("ls")

begin
p GLib::Spawn.command_line_sync("hoge")
rescue => e
  puts "class   = #{e.class}"
  puts "domain  = #{e.domain}"
  puts "code    = #{e.code}"
  puts "message = #{e.message}"
end
