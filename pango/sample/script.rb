=begin
  script.rb - Ruby/Pango sample script.

  Copyright (c) 2005 Ruby-GNOME2 Project
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: script.rb,v 1.1 2005/09/17 17:09:12 mutoh Exp $
=end

require 'pango'

iter = Pango::ScriptIter.new("Helloこんにちはコンニチハ안녕하세요السلام عليكمΕλληνικάவணக்கம்")
begin
  start_pos, end_pos, script = iter.range
  puts "-----"
  puts "start : #{start_pos}"
  puts "end   : #{end_pos}"
  puts "script: #{script.nick}"
  puts "sample_language: #{script.sample_language.to_s}"
end while iter.next!

