=begin
  item.rb - Ruby/Pango sample script.

  Copyright (c) 2005 Ruby-GNOME2 Project
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: item.rb,v 1.3 2006/06/17 13:37:01 mutoh Exp $
=end

require 'gtk2'

str = "Ruby-GNOME2こんにちわ" 
win = Gtk::Window.new("Ruby/Pango sample")
win.realize

p screen = Gdk::Screen.default
p context = Gdk::Pango.context(screen)
p attrlist = Pango::AttrList.new

p a = Pango::AttrBackground.new(65535, 0, 0)

attrlist.insert(Pango::AttrBackground.new(65535, 0,0))

attrs = [Pango::AttrBackground.new(65535, 0,0), Pango::AttrForeground.new(65535, 0,0)]

items = context.itemize(Pango::Context::DIRECTION_RTL, str, 0, 40, attrlist)

items.each do |v|
  ana = v.analysis
  puts "shape_engine = #{ana.shape_engine}, lang_engine = #{ana.lang_engine}, font = #{ana.font}, level = #{ana.level}"
  puts "language = #{ana.language}"
  p ana.font.get_glyph_extents("a"[0])[0].to_a
  ana.set_extra_attrs(attrs)
  p ana.extra_attrs
end
