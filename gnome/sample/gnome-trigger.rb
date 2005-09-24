=begin
  gnome-trigger.rb - a sample script for Gnome::Trigger.

  Copyright (C) 2005 Masao Mutoh
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: gnome-trigger.rb,v 1.2 2005/09/24 18:22:13 mutoh Exp $
=end

require 'gnome2'

Gnome::Trigger.set_action_func {|msg, level, supinfo|
  p msg
  p level
  p supinfo
}

trig = Gnome::Trigger.new(Gnome::Trigger::FUNCTION, "level1")
trig.add_trigger("gnome", "logout")

Gnome::Trigger.do("Trigger message", "level1", "gnome", "logout")
