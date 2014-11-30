module Gdk
  extend GLib::Deprecatable
  define_deprecated_enums :GrabStatus, 'GRAB'
  define_deprecated_enums :Status
  define_deprecated_const :Image, :raise => "Use 'Gdk::Pixbuf' or 'Cairo::Surface' instead."
  define_deprecated_const :Colormap, :raise => "Use 'Gdk::Visual' instead."
  define_deprecated_const :Input, :raise => "Use 'GLib::IOChannel' instead."
  define_deprecated_const :X11, 'GdkX11'

  class Cursor
    extend GLib::Deprecatable
    define_deprecated_enums :Type
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
    define_deprecated_enums :ExtensionMode, 'EXTENSION_EVENTS'
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
    define_deprecated_flags :Action, 'ACTION'
    define_deprecated_enums :Protocol, 'PROTO'
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
    define_deprecated_const :Mask, "Gdk::EventMask"
    define_deprecated_const :Type, "Gdk::EventType"
  end

  class EventCrossing
    extend GLib::Deprecatable
    define_deprecated_enums :Mode
    define_deprecated_enums :NotifyType, 'NOTIFY'
  end

  class EventOwnerChange
    extend GLib::Deprecatable
    define_deprecated_enums :OwnerChange
  end

  class EventProperty
    extend GLib::Deprecatable
    define_deprecated_enums :State, 'PROPERTY'
  end

  class EventScroll
    extend GLib::Deprecatable
    define_deprecated_enums :Direction
  end

  class EventSetting
    extend GLib::Deprecatable
    define_deprecated_enums :Action, 'ACTION'
  end

  class EventVisibility
    extend GLib::Deprecatable
    define_deprecated_enums :State
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
    define_deprecated_singleton_method :create_from_xpm, :raise => "Use 'Gdk::Pixbuf.new'."
    define_deprecated_singleton_method :create_from_xpm_d, :raise => "Use 'Gdk::Pixbuf.new'."
    define_deprecated_singleton_method :colormap_create_from_xpm, :raise => "Use 'Gdk::Pixbuf.new'."
    define_deprecated_singleton_method :colormap_create_from_xpm_d, :raise => "Use 'Gdk::Pixbuf.new'."
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
    define_deprecated_enums :Type
  end

  class Window
    extend GLib::Deprecatable

    define_deprecated_const :Gravity, "Gdk::Gravity"
    define_deprecated_const :AttributesType, "Gdk::WindowAttributesType"
    define_deprecated_const :Edge, "Gdk::WindowEdge"
    define_deprecated_const :FilterReturn, "Gdk::FilterReturn"

    class Hints
      extend GLib::Deprecatable
      define_deprecated_flags "Gdk::WindowHints"
    end

    class ModifierType
      extend GLib::Deprecatable
      define_deprecated_flags "Gdk::ModifierType"
    end

    class Type
      extend GLib::Deprecatable
      define_deprecated_enums "Gdk::WindowType"
    end

    class TypeHint
      extend GLib::Deprecatable
      define_deprecated_enums "Gdk::WindowTypeHint"
    end

    class WindowClass
      extend GLib::Deprecatable
      define_deprecated_enums "Gdk::WindowWindowClass"
    end

    class Class
      extend GLib::Deprecatable
      define_deprecated_enums "Gdk::WindowWindowClass"
    end

    class WMDecoration
      extend GLib::Deprecatable
      define_deprecated_flags "Gdk::WMDecoration"
    end

    class WMFunction
      extend GLib::Deprecatable
      define_deprecated_flags "Gdk::WMFunction"
    end

    define_deprecated_const :PARENT_RELATIVE, "Gdk::PARENT_RELATIVE"
    define_deprecated_method :clear, :warn => "Don't use this method."
    define_deprecated_method :clear_area, :warn => "Don't use this method."
    define_deprecated_method :internal_paint_info, :raise => "Don't use this method."
    define_deprecated_method :shape_combine_mask, :warn => "Don't use this method."
    define_deprecated_method :input_shape_combine_mask, :warn => "Don't use this method."
    define_deprecated_method :set_back_pixmap, :warn => "Don't use this method."
  end

  class WindowAttr
    extend GLib::Deprecatable
    define_deprecated_method :colormap, :raise => "Don't use this method."
    define_deprecated_method :set_colormap, :warn => "Don't use this method."
    alias :colormap= :set_colormap
  end
end

