=begin
  timeout2.rb - Sample for GLib::Timeout, GLib::MainLoop.

  Copyright (C) 2005 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Date: 2005/03/13 15:43:32 $
  $Id: timeout2.rb,v 1.2 2005/03/13 15:43:32 mutoh Exp $
=end

require 'glib2'

context = GLib::MainContext.new

mainloop = GLib::MainLoop.new(context, true)

source = GLib::Timeout.source_new(1000)

i = 0
source.set_callback {
  i += 1
  p "timeout2-#{i}"
  if i > 9
    mainloop.quit 
    false # the source is removed.
  else
    true  # continue ...
  end

}
source.attach(context)

mainloop.run

p "quit..."
