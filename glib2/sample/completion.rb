=begin
  completion.rb - Sample for GLib::Completion

  Copyright (C) 2005 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: completion.rb,v 1.1 2005/10/14 19:10:07 mutoh Exp $
=end

require 'glib2'

###########################
# Simple String completion
###########################
ary = ["a_", "a_ab", "aac", "aabc", "aabcd"]
comp = GLib::Completion.new
comp.add_items(ary)
p comp.complete("a_")
p comp.complete("aab")

puts "-----------"


############################
# Class completion and UTF-8
############################
class Test
  def initialize(str)
    @str = str
  end
  def value
    @str
  end
end

ary2 = [Test.new("あ_"), Test.new("あ_あい"), 
  Test.new("ああab"), Test.new("ああabc"), Test.new("ああbcd")]

comp2 = GLib::Completion.new {|data|
  data.value
}

comp2.add_items(ary2)
puts %Q$["#{comp2.complete("あ_").join(', "')}"]$
puts %Q$["#{comp2.complete("ああa").join(', "')}"]$
