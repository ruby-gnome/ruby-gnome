=begin
  spawn.rb - Sample for GLib::Spawn

  Copyright (C) 2005 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Date: 2005/03/24 14:55:21 $
  $Id: spawn.rb,v 1.2 2005/03/24 14:55:21 mutoh Exp $
=end

require 'glib2'

p GLib::Spawn.command_line_sync("ls")

#Here is an example to catch GLib::SpawnError.
begin
p GLib::Spawn.command_line_sync("nonexist_app")
rescue => e
  puts "class   = #{e.class}"
  puts "domain  = #{e.domain}"
  puts "code    = #{e.code}"
  puts "message = #{e.message}"
end
