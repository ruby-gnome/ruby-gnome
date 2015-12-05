#
# pango.rb
# Copyright(C) 2005-2015 Ruby-GNOME2 Project.
#
# This program is licenced under the same
# license of Ruby-GNOME2.
#

require 'glib2'
require 'cairo'

base_dir = Pathname.new(__FILE__).dirname.dirname.expand_path
vendor_dir = base_dir + "vendor" + "local"
vendor_bin_dir = vendor_dir + "bin"
GLib.prepend_dll_path(vendor_bin_dir)
begin
  major, minor, _ = RUBY_VERSION.split(/\./)
  require "#{major}.#{minor}/pango.so"
rescue LoadError
  require "pango.so"
end

if vendor_dir.exist?
  begin
    require "gobject-introspection"
    vendor_girepository_dir = vendor_dir + "lib" + "girepository-1.0"
    GObjectIntrospection.prepend_typelib_path(vendor_girepository_dir)
  rescue LoadError
  end
end

module Pango
  LOG_DOMAIN = "Pango"

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
  targets = [[Layout::Alignment, "ALIGN_"], [AttrScale, "SCALE_"], 
    [Coverage::Level, "COVERAGE_"], [Context::Direction, "DIRECTION_"],
    [Layout::WrapMode, "WRAP_"], [FontDescription::FontMask, "FONT_MASK_"], 
    [FontDescription::Stretch, "STRETCH_"], [FontDescription::Style, "STYLE_"],
    [FontDescription::Variant, "VARIANT_"], [FontDescription::Weight, "WEIGHT_"], 
    [TabArray::TabAlign, "TAB_"],
    [AttrUnderline::Underline, "UNDERLINE_"]]

  targets << [Script, "SCRIPT_"] if defined? Script
  targets << [Layout::EllipsizeMode, "ELLIPSIZE_"] if defined? Layout::EllipsizeMode
  targets << [Renderer::Part, "PART_"] if defined? Renderer::Part

  targets.each do |klass, prefix|
    (klass.constants - klass.superclass.constants).each do |name|
      unless klass.const_get(name).is_a? Class
	const_set("#{prefix}#{name}", klass.const_get(name))
      end
    end
  end
  
  module Version
    MAJOR, MINOR, MICRO = BUILD_VERSION
    STRING = Pango.version_string

    class << self
      def or_later?(major, minor, micro = nil)
        micro || 0
        error_message = Pango.check_version(major, minor, micro)
        error_message.nil?
      end
    end
  end
end

GLib::Log.set_log_domain(Pango::LOG_DOMAIN)
