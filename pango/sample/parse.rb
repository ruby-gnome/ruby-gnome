#!/usr/local/bin/ruby
=begin
  parse.rb - Ruby/Pango sample script.

  Copyright (c) 2002 Ruby-GNOME2 Project
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: parse.rb,v 1.1 2003/01/03 16:34:48 mutoh Exp $
=end

require 'pango'

str = File.read("sample.txt")
puts str
puts "------"

attr_list, text, accel_char = Pango.parse_markup(str, '$')
puts "attr_list = #{attr_list}"
puts "text = #{text}"
puts "accel_char = #{accel_char}"
puts "------"

iterator = attr_list.iterator

attr_list.each(text) do |iterator, text, start_pos, end_pos|
  puts "range = #{start_pos}, #{end_pos}"
  puts "text = #{text}"
  iterator.get.each do |attr|
    puts "#{attr} = #{attr.value}"
  end
  puts "---"
end

