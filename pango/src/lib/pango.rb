require 'glib2'
begin
  require 'cairo'
rescue LoadError
end
require 'pango.so'


module Pango
  LOG_DOMAIN = "Pango"

  def cairo_available?
    Pango.constants.include?("CairoFontMap")
  end
  module_function :cairo_available?

  class AttrList
    def each(text = nil)
      iter = iterator
      begin
        if text
          s, e = iter.range
          yield(iter, text[s, e - s], s, e)
        else
          yield(iter)
        end
     end while iter.next!
    end
  end

  # [[klass, prefix], ....]]
  [[Layout::Alignment, "ALIGN_"], [AttrScale, "SCALE_"], [Script, "SCRIPT_"],
    [Coverage::Level, "COVERAGE_"], [Context::Direction, "DIRECTION_"],
    [Layout::EllipsizeMode, "ELLIPSIZE_"], [Layout::WrapMode, "WRAP_"],
    [FontDescription::FontMask, "FONT_MASK_"], 
    [FontDescription::Stretch, "STRETCH_"], [FontDescription::Style, "STYLE_"],
    [FontDescription::Variant, "VARIANT_"], [FontDescription::Weight, "WEIGHT_"], 
    [Renderer::Part, "PART_"], [TabArray::TabAlign, "TAB_"],
    [AttrUnderline::Underline, "UNDERLINE_"]] .each do |klass, prefix|
    klass.constants.each do |name|
      unless klass.const_get(name).is_a? Class
	const_set(prefix + name, klass.const_get(name))
      end
    end
  end
end

GLib::Log.set_log_domain(Pango::LOG_DOMAIN)
