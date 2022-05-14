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

module Gdk
  extend GLib::Deprecatable
  define_deprecated_const :Atom, raise: "No longer available in GDK 4."
  define_deprecated_const :Color, raise: "No longer available in GDK 4."
  define_deprecated_const :Colormap, raise: "Use 'Gdk::Visual' instead."
  define_deprecated_const :Color, raise: "No longer available in GDK 4."
  define_deprecated_const :DragContext, raise: "No longer available in GDK 4."
  define_deprecated_const :EventCrossing, "Gdk::CrossingEvent"
  define_deprecated_const :EventMask, raise: "No longer available in GDK 4."
  define_deprecated_const :EventOwnerChange, raise: "No longer available in GDK 4."
  define_deprecated_const :EventProperty, raise: "No longer available in GDK 4."
  define_deprecated_const :EventScroll, "Gdk::ScrollEvent"
  define_deprecated_const :EventSetting, raise: "No longer available in GDK 4."
  define_deprecated_const :EventVisibility, raise: "No longer available in GDK 4."
  define_deprecated_const :EventWindowState, raise: "No longer available in GDK 4."
  define_deprecated_const :FilterReturn, raise: "No longer available in GDK 4."
  define_deprecated_const :GrabStatus, raise: "No longer available in GDK 4."
  define_deprecated_const :Image,
                          raise: "Use 'GdkPixbuf::Pixbuf' or 'Cairo::Surface' instead."
  define_deprecated_const :Input, raise: "Use 'GLib::IOChannel' instead."
  define_deprecated_const :Property, raise: "No longer available in GDK 4."
  define_deprecated_const :Screen, raise: "No longer available in GDK 4."
  define_deprecated_const :Selection, raise: "No longer available in GDK 4."
  define_deprecated_const :Status, raise: "No longer available in GDK 4."
  define_deprecated_const :Visual, raise: "No longer available in GDK 4."
  define_deprecated_const :Window, raise: "Use 'Gdk::Surface' instead."
  define_deprecated_const :WindowAttr, raise: "No longer available in GDK 4."
  define_deprecated_const :WindowAttributesType,
                          raise: "No longer available in GDK 4."
  define_deprecated_const :WindowHints, raise: "No longer available in GDK 4."
  define_deprecated_const :WindowType, raise: "No longer available in GDK 4."
  define_deprecated_const :WindowTypeHint, raise: "No longer available in GDK 4."
  define_deprecated_const :WindowWindowClass,
                          raise: "No longer available in GDK 4."
  define_deprecated_const :WMDecoration, raise: "No longer available in GDK 4."
  define_deprecated_const :WMFunction, raise: "No longer available in GDK 4."
  define_deprecated_const :X11, 'GdkX11'

  class Cursor
    extend GLib::Deprecatable
    define_deprecated_const :CursorType, raise: "No longer available in GDK 4."
    define_deprecated_method :pixmap?, :warn => "Don't use this method." do |_self|
      false
    end
  end

  class Device
    extend GLib::Deprecatable
    define_deprecated_enums :AxisUse, 'AXIS'
    define_deprecated_const :EXTENSION_EVENTS_NONE,
                            raise: "No longer available in GDK 4."
    define_deprecated_const :EXTENSION_EVENTS_ALL,
                            raise: "No longer available in GDK 4."
    define_deprecated_const :EXTENSION_EVENTS_CURSOR,
                            raise: "No longer available in GDK 4."
    define_deprecated_const :ExtensionMode,
                            raise: "No longer available in GDK 4."
    define_deprecated_const :InputMode, raise: "No longer available in GDK 4."
    define_deprecated_enums :InputSource, 'SOURCE'
  end

  class Display
    extend GLib::Deprecatable
    define_deprecated_method :double_click_time, :raise => "Don't use this method."
    define_deprecated_method :double_click_distance, :raise => "Don't use this method."
    define_deprecated_method :button_click_time, :raise => "Don't use this method."
    define_deprecated_method :button_window, :raise => "Don't use this method."
    define_deprecated_method :button_number, :raise => "Don't use this method."
    define_deprecated_method :button_x, :raise => "Don't use this method."
    define_deprecated_method :button_y, :raise => "Don't use this method."
  end

  class Event
    extend GLib::Deprecatable

    define_deprecated_const :CURRENT_TIME, "Gdk::CURRENT_TIME"

    define_deprecated_const :Mask, raise: "No longer available in GDK 4."

    define_deprecated_const :Type, "Gdk::EventType"
    define_deprecated_enums "Gdk::EventType"
  end

  module Keyval
    extend GLib::Deprecatable
    constants.each do |key|
      old_names = []
      old_names << key.to_s.sub(/^KEY_/, 'GDK_KEY_')
      old_names << key.to_s.sub(/^KEY_/, 'GDK_')
      old_names.each do |old_name|
        define_deprecated_const old_name, [self, key].join('::')
      end
    end
  end

  class Pixmap
    extend GLib::Deprecatable
    define_deprecated_method :initialize, :raise => "Use 'Gdk::Window#create_similar_surface'."
    define_deprecated_singleton_method :create_from_data, :raise => "Use 'Cairo::ImageSurface.new'."
    define_deprecated_singleton_method :create_from_xbm, :raise => "Use 'Cairo::ImageSurface.new'."
    define_deprecated_singleton_method :create_from_xpm, :raise => "Use 'GdkPixbuf::Pixbuf.new'."
    define_deprecated_singleton_method :create_from_xpm_d, :raise => "Use 'GdkPixbuf::Pixbuf.new'."
    define_deprecated_singleton_method :colormap_create_from_xpm, :raise => "Use 'GdkPixbuf::Pixbuf.new'."
    define_deprecated_singleton_method :colormap_create_from_xpm_d, :raise => "Use 'GdkPixbuf::Pixbuf.new'."
    define_deprecated_singleton_method :foreign_new, :raise => "Don't use this method."
    define_deprecated_singleton_method :lookup, :raise => "Don't use this method."
  end

  class Surface
    extend GLib::Deprecatable

    define_deprecated_const :Gravity, "Gdk::Gravity"
    define_deprecated_enums "Gdk::Gravity"
    define_deprecated_const :AttributesType,
                            raise: "No longer available in GDK 4."
    define_deprecated_const :Edge, "Gdk::SurfaceEdge"
    define_deprecated_enums "Gdk::SurfaceEdge"
    define_deprecated_const :FilterReturn,
                            raise: "No longer available in GDK 4."
    define_deprecated_const :Hints, raise: "No longer available in GDK 4."
    define_deprecated_const :ModifierType, "Gdk::ModifierType"
    define_deprecated_enums "Gdk::ModifierType"
    define_deprecated_const :Type, raise: "No longer available in GDK 4."
    define_deprecated_const :TypeHint, raise: "No longer available in GDK 4."
    define_deprecated_const :Class, raise: "No longer available in GDK 4."
    define_deprecated_const :WMDecoration, raise: "No longer available in GDK 4."
    define_deprecated_const :WMFunction, raise: "No longer available in GDK 4."
    define_deprecated_const :PARENT_RELATIVE, "Gdk::PARENT_RELATIVE"
    define_deprecated_method :clear, :warn => "Don't use this method."
    define_deprecated_method :clear_area, :warn => "Don't use this method."
    define_deprecated_method :internal_paint_info, :raise => "Don't use this method."
    define_deprecated_method :shape_combine_mask, :warn => "Don't use this method."
    define_deprecated_method :input_shape_combine_mask, :warn => "Don't use this method."
    define_deprecated_method :set_back_pixmap, :warn => "Don't use this method."
    define_deprecated_method :size, :warn => "Use #width and #height instead." do |_self|
      [_self.width, _self.height]
    end
  end
end

