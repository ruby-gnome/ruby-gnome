=begin
  attribute.rb - Ruby/Pango sample script.

  Copyright (c) 2002,2003 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: attribute.rb,v 1.6 2006/06/17 13:37:01 mutoh Exp $
=end

require 'pango'

p a = Pango::AttrSize.new(1)
puts "value = #{a.value}"
puts "start_index = #{a.start_index}"
puts "end_index = #{a.end_index}"

p a = Pango::AttrStyle.new(Pango::FontDescription::STYLE_NORMAL)
puts "value = #{a.value}"

p a = Pango::AttrVariant.new(Pango::FontDescription::VARIANT_SMALL_CAPS)
puts "value = #{a.value}"

p a = Pango::AttrStretch.new(Pango::FontDescription::STRETCH_EXPANDED)
puts "value = #{a.value}"

p a = Pango::AttrWeight.new(Pango::FontDescription::WEIGHT_BOLD)
puts "value = #{a.value}"

p a = Pango::AttrRise.new(10)
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

p Pango::AttrUnderline::SINGLE
p a = Pango::AttrUnderline.new(Pango::AttrUnderline::SINGLE)
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
  p a = Gdk::PangoAttrEmbossed.new(true)
  p a.value

  mask = Gdk::Pixmap.new(Gtk::Window.new.realize.window, 100, 100, 1)
  p a = Gdk::PangoAttrStipple.new(mask)
  p a.value


