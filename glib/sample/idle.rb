=begin
  idle.rb - Sample for GLib::Idle, GLib::MainLoop.

  Copyright (C) 2005 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Date: 2005/03/13 14:39:58 $
  $Id: idle.rb,v 1.1 2005/03/13 14:39:58 mutoh Exp $
=end

require 'glib2'

mainloop = GLib::MainLoop.new(nil, true)

i = 0
GLib::Idle.add {
  i += 1
  p "timeout1-#{i}"
  if i > 9
    mainloop.quit 
    false # the source is removed.
  else
    true  # continue ...
  end
}

j = 0
GLib::Idle.add {
  j += 1
  p "timeout2-#{i}"
  if j > 9
    mainloop.quit 
    false # the source is removed.
  else
    true  # continue ...
  end
}

mainloop.run

p "quit..."
