=begin
  timer.rb - Sample for GLib::Timer

  Copyright (C) 2005 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: timer.rb,v 1.1 2005/10/14 19:48:41 mutoh Exp $
=end

require 'glib2'

timer = GLib::Timer.new

timer.start
puts "start (status = running) : #{timer.elapsed}"

sleep(3)

puts "after 3 sec (status = running) : #{timer.elapsed}"

sleep(3)

puts "after 3 sec (status = running) : #{timer.elapsed}"
timer.stop
puts "stop (status = stoped) : #{timer.elapsed}"

sleep(3)
puts "after 3 sec (status = stoped) : #{timer.elapsed}"

timer.continue
puts "continue (status = running) : #{timer.elapsed}"
sleep(3)
puts "after 3 sec (status = running) : #{timer.elapsed}"

timer.reset
puts "reset (status = running) : #{timer.elapsed}"
sleep(3)
puts "after 3 sec (status = running) : #{timer.elapsed}"


