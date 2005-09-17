=begin
  break.rb - Ruby/Pango sample script.

  Copyright (c) 2005 Ruby-GNOME2 Project
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: break.rb,v 1.1 2005/09/17 17:09:12 mutoh Exp $
=end

require 'pango'

attr = Pango.break("Hello Worldです\n2nd line", Pango::Analysis.new)

p attr.length
attr.each do |v|
  print "line_break:#{v.line_break?}"
  print ", mandatory_break:#{v.mandatory_break?}"
  print ", char_break:#{v.char_break?}"
  print ", white:#{v.white?}"
  print ", cursor_position:#{v.cursor_position?}"
  print ", word_start:#{v.word_start?}"
  print ", word_end:#{v.word_end?}"
  print ", sentence_boundary:#{v.sentence_boundary?}"
  print ", sentence_start:#{v.sentence_start?}"
  print ", sentence_end:#{v.sentence_end?}\n"
end
