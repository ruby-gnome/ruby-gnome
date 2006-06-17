#!/usr/bin/env ruby
=begin
  glyphstring.rb - Ruby/Pango sample script.

  Copyright (c) 2002-2006 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: glyphstring.rb,v 1.3 2006/06/17 14:16:53 ktou Exp $
=end

require 'gtk2'

str = "Hello World"

lang = Pango::Language.new("ja_JP")
fd = Pango::FontDescription.new("Sans 18")

context = Gdk::Pango.context
context.language = lang
context.base_dir = Pango::DIRECTION_LTR
context.font_description = fd

p font = context.load_font(fd)
family = context.families[10]
puts "#{family.class}, #{family.name}"

family.faces.each do |face|
  puts face.name
end

p fmap = font.font_map

p fset = fmap.load_fontset(context, fd, lang)


gs = Pango::GlyphString.new

ink_rect, logical_rect = gs.extents(font)

p ink_rect.to_a
p logical_rect.to_a

item = Pango::Item.new
item.analysis.font = font
item.analysis.shape_engine = font.find_shaper(lang, str[0])

gs = Pango.shape(str, item.analysis)

ink_rect, logical_rect = gs.extents(font, 0, 11)

p ink_rect.to_a
p logical_rect.to_a

gs.glyphs.each do |info, log_clusters|
  puts "Pango::GlyphInfo: log_clusters = #{log_clusters}, glyph = #{info.glyph}, geometry = #{info.geometry.inspect}, cluster_start? = #{info.cluster_start?}"
end

p gs.index_to_x(str, item.analysis, 3, false)
p gs.x_to_index(str, item.analysis, 103)

p logical_widths = gs.get_logical_widths(str, item.analysis.level)
