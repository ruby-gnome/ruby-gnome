=begin
  shell.rb - Sample for GLib::Shell

  Copyright (C) 2005 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: shell.rb,v 1.1 2005/10/14 19:10:07 mutoh Exp $
=end

require 'glib2'

cmd = "ls *.c *.o"

p GLib::Shell.parse(cmd)

puts quote =  GLib::Shell.quote(cmd)
puts GLib::Shell.unquote(quote)

puts "----"

#Samples to catch an Exception
begin
  GLib::Shell.parse('foooo "bar')
rescue GLib::ShellError => e
  puts "domain  = #{e.domain}"
  puts "code    = #{e.code}"
  puts "message = #{e.message}"
end

begin
  GLib::Shell.unquote('foooo "bar')
rescue GLib::ShellError => e
  puts "domain  = #{e.domain}"
  puts "code    = #{e.code}"
  puts "message = #{e.message}"
end
