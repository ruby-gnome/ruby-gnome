# Copyright (C) 2011-2015  Ruby-GNOME2 Project Team
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
  define_deprecated_enums :GrabStatus, 'GRAB'
  define_deprecated_enums :Status
  define_deprecated_const :Image, :raise => "Use 'GdkPixbuf::Pixbuf' or 'Cairo::Surface' instead."
  define_deprecated_const :Colormap, :raise => "Use 'Gdk::Visual' instead."
  define_deprecated_const :Input, :raise => "Use 'GLib::IOChannel' instead."
  define_deprecated_const :X11, 'GdkX11'

  class Cursor
    extend GLib::Deprecatable
    define_deprecated_enums :CursorType
    define_deprecated_method :pixmap?, :warn => "Don't use this method." do |_self|
      false
    end
    Gdk::CursorType.constants.each do |cursor_type|
      define_deprecated_const cursor_type, ['Gdk::CursorType', cursor_type].join('::')
    end
  end

  class Device
    extend GLib::Deprecatable
    define_deprecated_enums :AxisUse, 'AXIS'
    define_deprecated_const "EXTENSION_EVENTS_NONE",
                            :raise => "No longer available in GDK3."
    define_deprecated_const "EXTENSION_EVENTS_ALL",
                            :raise => "No longer available in GDK3."
    define_deprecated_const "EXTENSION_EVENTS_CURSOR",
                            :raise => "No longer available in GDK3."
    define_deprecated_const "ExtensionMode",
                            :raise => "No longer available in GDK3."
    define_deprecated_enums :InputMode, 'MODE'
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

  class DragContext
    extend GLib::Deprecatable
    define_deprecated_flags :DragAction, 'ACTION'
    define_deprecated_const :Action, "Gdk::DragAction"
    define_deprecated_enums :DragProtocol, 'PROTO'
    define_deprecated_const :Protocol, "Gdk::DragProtocol"
    define_deprecated_singleton_method :drag_begin, :warn => "Use 'Gdk::Window#drag_begin'." do |_self, window, targets|
      window.drag_begin(targets)
    end
    define_deprecated_singleton_method :get_protocol, :raise => "Use 'Gdk::Window#drag_protocol'."
    define_deprecated_method :initialize, :raise => "Use 'Gdk::Window#drag_begin'."
    define_deprecated_method :action, :selected_action
    define_deprecated_method :source?, :warn => "Don't use this method."
    define_deprecated_method :start_time, :raise => "Don't use this method."
  end

  class Event
    extend GLib::Deprecatable

    define_deprecated_const :CURRENT_TIME, "Gdk::CURRENT_TIME"

    define_deprecated_const :Mask, "Gdk::EventMask"
    define_deprecated_enums "Gdk::EventMask"

    define_deprecated_const :Type, "Gdk::EventType"
    define_deprecated_enums "Gdk::EventType"
  end

  class EventCrossing
    extend GLib::Deprecatable
    define_deprecated_enums :CrossingMode
    define_deprecated_enums :NotifyType, 'NOTIFY'
  end

  class EventOwnerChange
    extend GLib::Deprecatable
    define_deprecated_enums :OwnerChange
  end

  class EventProperty
    extend GLib::Deprecatable
    define_deprecated_enums :PropertyState, 'PROPERTY'
  end

  class EventScroll
    extend GLib::Deprecatable
    define_deprecated_const :Direction, "Gdk::ScrollDirection"
    define_deprecated_enums "Gdk::ScrollDirection"
  end

  class EventSetting
    extend GLib::Deprecatable
    define_deprecated_enums :SettingAction, 'ACTION'
  end

  class EventVisibility
    extend GLib::Deprecatable
    define_deprecated_enums :VisibilityState
    define_deprecated_const :State, "Gdk::VisibilityState"
  end

  class EventWindowState
    extend GLib::Deprecatable
    define_deprecated_flags :WindowState
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

  module Property
    extend GLib::Deprecatable
    define_deprecated_enums :PropMode, 'MODE'
  end

  class Visual
    extend GLib::Deprecatable
    define_deprecated_enums :ByteOrder
    define_deprecated_enums :VisualType
    define_deprecated_const :Type, "Gdk::VisualType"
  end

  class Window
    extend GLib::Deprecatable

    define_deprecated_const :Gravity, "Gdk::Gravity"
    define_deprecated_enums "Gdk::Gravity"
    define_deprecated_const :AttributesType, "Gdk::WindowAttributesType"
    define_deprecated_enums "Gdk::WindowAttributesType"
    define_deprecated_const :Edge, "Gdk::WindowEdge"
    define_deprecated_enums "Gdk::WindowEdge"
    define_deprecated_const :FilterReturn, "Gdk::FilterReturn"
    define_deprecated_enums "Gdk::FilterReturn"
    define_deprecated_const :Hints, "Gdk::WindowHints"
    define_deprecated_enums "Gdk::WindowHints"
    define_deprecated_const :ModifierType, "Gdk::ModifierType"
    define_deprecated_enums "Gdk::ModifierType"
    define_deprecated_const :Type, "Gdk::WindowType"
    define_deprecated_enums "Gdk::WindowType"
    define_deprecated_const :TypeHint, "Gdk::WindowTypeHint"
    define_deprecated_enums "Gdk::WindowTypeHint"
    define_deprecated_const :Class, "Gdk::WindowWindowClass"
    define_deprecated_enums "Gdk::WindowWindowClass"
    define_deprecated_const :WMDecoration, "Gdk::WMDecoration"
    define_deprecated_enums "Gdk::WMDecoration"
    define_deprecated_const :WMFunction, "Gdk::WMFunction"
    define_deprecated_enums "Gdk::WMFunction"
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

  class WindowAttr
    extend GLib::Deprecatable
    define_deprecated_method :colormap, :raise => "Don't use this method."
    define_deprecated_method :set_colormap, :warn => "Don't use this method."
    alias :colormap= :set_colormap
  end
end

