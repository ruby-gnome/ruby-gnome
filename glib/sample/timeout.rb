=begin
  timeout.rb - Sample for GLib::Timeout, GLib::MainLoop.

  Copyright (C) 2005 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: timeout.rb,v 1.2 2005/07/14 17:05:22 mutoh Exp $
=end

require 'glib2'

mainloop = GLib::MainLoop.new(nil, true)

i = 0
GLib::Timeout.add(1000) {
  i += 1
  p "timeout-#{i}"
  if i > 9
    mainloop.quit 
    false # the source is removed.
  else
    true  # continue ...
  end
}

mainloop.run

p "quit..."
