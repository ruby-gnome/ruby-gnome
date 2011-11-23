module Gdk
  extend GLib::Deprecatable
  define_deprecated_enums GrabStatus, 'GRAB'
  define_deprecated_enums Status
  define_deprecated_const :Image, :raise => "Use 'Gdk::Pixbuf' or 'Cairo::Surface' instead."
  define_deprecated_const :Colormap, :raise => "Use 'Gdk::Visual' instead."
  define_deprecated_const :Input, :raise => "Use 'GLib::IOChannel' instead."

  class Cursor
    extend GLib::Deprecatable
    define_deprecated_enums Type
    define_deprecated_method :pixmap?, :warn => "Don't use this method." do |_self|
      false
    end
  end

  class Device
    extend GLib::Deprecatable
    define_deprecated_enums AxisUse, 'AXIS'
    define_deprecated_enums ExtensionMode, 'EXTENSION_EVENTS'
    define_deprecated_enums InputMode, 'MODE'
    define_deprecated_enums InputSource, 'SOURCE'
    define_deprecated_method :source, :input_source
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
    define_deprecated_flags Action, 'ACTION'
    define_deprecated_enums Protocol, 'PROTO'
  end

  class Event
    extend GLib::Deprecatable
    define_deprecated_flags Mask
    define_deprecated_enums Type
  end

  class EventCrossing
    extend GLib::Deprecatable
    define_deprecated_enums Mode
    define_deprecated_enums NotifyType, 'NOTIFY'
  end

  class EventOwnerChange
    extend GLib::Deprecatable
    define_deprecated_enums OwnerChange
  end

  class EventProperty
    extend GLib::Deprecatable
    define_deprecated_enums State, 'PROPERTY'
  end

  class EventScroll
    extend GLib::Deprecatable
    define_deprecated_enums Direction
  end

  class EventSetting
    extend GLib::Deprecatable
    define_deprecated_enums Action, 'ACTION'
  end

  class EventVisibility
    extend GLib::Deprecatable
    define_deprecated_enums State
  end

  class EventWindowState
    extend GLib::Deprecatable
    define_deprecated_flags WindowState
  end

  module Keyval
    extend GLib::Deprecatable
    constants.each do |key|
      define_deprecated_const key.to_s.sub(/^GDK_KEY_/, 'GDK_'), [self, key].join('::')
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

  class Property
    extend GLib::Deprecatable
    define_deprecated_enums PropMode, 'MODE'
  end

  class Visual
    extend GLib::Deprecatable
    define_deprecated_enums ByteOrder
    define_deprecated_enums Type
  end

  class Window
    extend GLib::Deprecatable
    define_deprecated_flags AttributesType, 'WA'
    define_deprecated_enums Edge, 'EDGE'
    define_deprecated_enums FilterReturn, 'FILTER'
    define_deprecated_enums Gravity, 'GRAVITY'
    define_deprecated_flags Hints, 'HINT'
    define_deprecated_flags ModifierType
    define_deprecated_enums Type
    define_deprecated_enums TypeHint, 'TYPE_HINT'
    define_deprecated_flags WMDecoration, 'DECOR'
    define_deprecated_flags WMFunction, 'FUNC'
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

