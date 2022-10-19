# Copyright (C) 2011-2022  Ruby-GNOME Project Team
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

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
  extend GLib::Deprecatable
  define_deprecated_const :DrawSpacesFlags, "GtkSource::SpaceTypeFlags"

  class Buffer
    extend GLib::Deprecatable
    define_deprecated_method :not_undoable_action, :begin_not_undoable_action
    define_deprecated_method :non_undoable_action, :begin_not_undoable_action
  end

  class View
    extend GLib::Deprecatable
    define_deprecated_enums :SmartHomeEndType, 'SMART_HOME_END'
    define_deprecated_const :BUILD_VERSION, 'GtkSource::BUILD_VERSION'
    define_deprecated_method :set_mark_category_background, :warn => "Use 'GtkSource::MarkAttributes#set_background'." do |_self, category, background|
      _self.get_mark_attributes(category).first.set_background(background) # TODO
    end
    define_deprecated_method :get_mark_category_background, :warn => "Use 'GtkSource::MarkAttributes#background'." do |_self, category|
      _self.get_mark_attributes(category).first.background # TODO
    end
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

