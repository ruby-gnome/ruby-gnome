=begin
  attribute.rb - Ruby/Pango sample script.

  Copyright (c) 2002,2003 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: attribute.rb,v 1.2 2003/02/01 17:13:24 mutoh Exp $
=end

require 'pango'

p a = Pango::AttrSize.new(1)
puts "value = #{a.value}"
puts "start_index = #{a.start_index}"
puts "end_index = #{a.end_index}"

p a = Pango::AttrStyle.new(2)
puts "value = #{a.value}"

p a = Pango::AttrVariant.new(3)
puts "value = #{a.value}"

p a = Pango::AttrStretch.new(4)
puts "value = #{a.value}"

p a = Pango::AttrWeight.new(5)
puts "value = #{a.value}"

p a = Pango::AttrRise.new(6)
puts "value = #{a.value}"

p a = Pango::AttrLanguage.new(Pango::Language.new("ja"))
p a.value
p a.value.to_str

p a = Pango::AttrFamily.new("family")
p a.value

p b = Pango::FontDescription.new
p b.to_str
p a = Pango::AttrFontDescription.new(b)
p a.value
p a.value.to_str

p a = Pango::AttrForeground.new(10, 20, 30)
p a.value
p a.value.to_a

p a = Pango::AttrBackground.new(20, 30, 40)
p a.value
p a.value.to_a

p a = Pango::AttrStrikethrough.new(true)
p a.value

p Pango::Attribute::UNDERLINE_SINGLE
p a = Pango::AttrUnderline.new(Pango::Attribute::UNDERLINE_SINGLE)
p a.value

p ink = Pango::Rectangle.new(1, 2, 3, 4)
p log = Pango::Rectangle.new(5, 6, 7, 8)
p a = Pango::AttrShape.new(ink, log)
p a.value[0].to_a
p a.value[1].to_a

p a = Pango::AttrScale.new(2.3)
p a.value

begin
  require 'gtk2'
rescue
  p "Ruby/GTK2 is not existed."
  exit 0
end
  Gtk.init

  p a = Gdk::Pango::AttrEmbossed.new(true)
  p a.value

  mask = Gdk::Pixmap.new(Gtk::Window.new.realize.window, 100, 100, 1)
  p a = Gdk::Pango::AttrStipple.new(mask)
  p a.value


