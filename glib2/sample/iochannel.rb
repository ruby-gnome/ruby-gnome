=begin
  iochannel.rb - Sample for GLib::IOChannel.

  Copyright (C) 2005 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: iochannel.rb,v 1.3 2006/12/20 18:08:20 mutoh Exp $
=end

require 'glib2'

path = ARGV[0] || __FILE__

GLib::IOChannel.open(path) {|io|
  puts io.read
}

stdout = GLib::IOChannel.new(path, "r")
stdout.add_watch(GLib::IOChannel::IN
		 ) {|io, condition|
  puts "condition = #{condition}"
  false
}

context = GLib::MainContext.default
mainloop = GLib::MainLoop.new(context, true)


Thread.new{
  num = 0
  loop {
    num += 1
    str = stdout.gets
    puts "line #{num}: #{str}"
    unless str
      mainloop.quit
      break
    end
  }
}

mainloop.run

stdout.close
