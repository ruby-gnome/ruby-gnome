=begin
  gnome-sound.rb - a sample script for Gnome::Sound.

  Copyright (C) 2005 Masao Mutoh
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: gnome-sound.rb,v 1.1 2005/09/24 18:02:43 mutoh Exp $
=end

require 'gnome2'

unless ARGV[0]
  puts "USAGE: ruby gnome-sound.rb foo.wav"
  exit
end

Gnome::Sound.init("localhost")

Gnome::Sound.play(ARGV[0])

Gnome::Sound.shutdown
