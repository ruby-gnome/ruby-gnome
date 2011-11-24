module Gtk
  extend GLib::Deprecatable
  define_deprecated_const :SourceBuffer, 'GtkSource::Buffer'
  define_deprecated_const :SourceLanguage, 'GtkSource::Language'
  define_deprecated_const :SourceLanguageManager, 'GtkSource::LanguageManager'
  define_deprecated_const :SourceMark, 'GtkSource::Mark'
  define_deprecated_const :SourcePrintCompositor, 'GtkSource::PrintCompositor'
  define_deprecated_const :SourceStyle, 'GtkSource::Style'
  define_deprecated_const :SourceStyleScheme, 'GtkSource::StyleScheme'
  define_deprecated_const :SourceStyleSchemeManager, 'GtkSource::StyleSchemeManager'
  define_deprecated_const :SourceView, 'GtkSource::View'
end

module GtkSource
  class View
    extend GLib::Deprecatable
    define_deprecated_flags :DrawSpacesFlags, 'DRAW_SPACES'
    define_deprecated_enums :SmartHomeEndType, 'SMART_HOME_END'
    define_deprecated_const :BUILD_VERSION, 'GtkSource::BUILD_VERSION'
    define_deprecated_method :set_mark_category_pixbuf, :warn => "Use 'GtkSource::MarkAttributes#set_pixbuf'." do |_self, category, pixbuf|
      _self.get_mark_attributes(category).first.set_pixbuf(pixbuf) # TODO
    end
    define_deprecated_method :get_mark_category_pixbuf, :warn => "Use 'GtkSource::MarkAttributes#pixbuf'." do |_self, category|
      _self.get_mark_attributes(category).first.pixbuf # TODO
    end
    define_deprecated_method :set_mark_category_priority, :warn => "Use '#{self}#set_mark_attributes'." do |_self, category, priority|
      attributes = _self.get_mark_attributes(category).first # TODO
      _self.set_mark_attributes(category, attributes, priority)
    end
    define_deprecated_method :get_mark_category_priority, :warn => "Use '#{self}#get_mark_attributes'." do |_self, category|
      _self.get_mark_attributes(category).last # TODO
    end
  end
end

