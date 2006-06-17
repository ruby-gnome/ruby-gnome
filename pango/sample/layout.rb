#!/usr/local/bin/ruby
=begin
  layout.rb - Ruby/Pango sample script.

  Copyright (c) 2005 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: layout.rb,v 1.2 2006/06/17 13:36:11 mutoh Exp $
=end

require 'gtk2'
require 'pp'

puts
puts "Start----------"
puts
str = File.readlines("sample.txt").to_s
context = Gdk::Pango.context
context.base_dir = Pango::Context::DIRECTION_LTR
context.language = Pango::Language.new("ja_JP")
fd = Pango::FontDescription.new("Sans 18")
font = context.load_font(fd)
font_map = context.font_map 

p "context: #{context}"
p "font_desc: #{fd}"
p "font: #{font}"
p "font_map: #{font_map}"

puts
puts "Pango::Layout----------"
puts
p layout = Pango::Layout.new(context)

layout.set_markup(str)
layout.font_description = fd
layout.auto_dir = true
layout.alignment = Pango::Layout::ALIGN_LEFT

p "attributes: #{layout.attributes}"
p "font_description: #{layout.font_description}"
p "width: #{layout.width}"
p "wrap: #{layout.wrap}"
p "ellipsize: #{layout.ellipsize}"
p "indent: #{layout.indent}"
p "spacing: #{layout.spacing}"
p "justify?: #{layout.justify?}"
p "auto_dir?: #{layout.auto_dir?}"
p "alignment: #{layout.alignment.inspect}"
p "tabs: #{layout.tabs}"
p "single_paragraph_mode?: #{layout.single_paragraph_mode?}"
p "size: #{layout.size}"
p "pixel_size: #{layout.pixel_size}"
p "line_count: #{layout.line_count}"

puts "lines:" 
pp layout.lines

puts
puts "Pango::LayoutIter--------"
puts
iter = layout.iter
p "iter: #{iter}"
p "index: #{iter.index}"
p "baseline: #{iter.baseline}"
p "line: #{iter.line}"
p "char_extents: #{iter.char_extents}"
p "cluster_extents: #{iter.cluster_extents}"
p "run_extents: #{iter.run_extents}"
p "layout_extents: #{iter.layout_extents}"
p "line_extents: #{iter.line_extents}"
p "line_yrange: #{iter.line_yrange}"

puts
puts "Pango::GlyphItem-------"
puts
glyph = iter.run
p "glyph: #{glyph}"
p "item: #{glyph.item}"
p "glyphs: #{glyph.glyphs}"

puts
puts "Pango::LayoutLine ----------"
puts

line = iter.line

p "line = #{line}"
p "extents: #{line.extents}"
p "pixel_extents: #{line.pixel_extents}"
p "index_to_x: #{line.index_to_x(1, true)}"
p "x_to_index: #{line.x_to_index(1)}"
p "x_ranges: #{line.get_x_ranges(0, 2)}"
p "layout: #{line.layout}"
p "start_index: #{line.start_index}"
p "length: #{line.length}"
p "paragraph_start?: #{line.paragraph_start?}"
p "resolved_dir: #{line.resolved_dir}"
puts "runs:"
pp line.runs
puts
puts "End----------"
