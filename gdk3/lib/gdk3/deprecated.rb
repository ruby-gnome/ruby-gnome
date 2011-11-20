module Gdk
  extend GLib::Deprecatable
  define_deprecated_const :Pixmap, :raise => "Use 'Cairo::Surface' instead."
  define_deprecated_const :Image, :raise => "Use 'Gdk::Pixbuf' or 'Cairo::Surface' instead."
  define_deprecated_const :Colormap, :raise => "Use 'Gdk::Visual' instead."
  define_deprecated_const :Input, :raise => "Use 'GLib::IOChannel' instead."

  class Cursor
    extend GLib::Deprecatable
    define_deprecated_method :pixmap?, :warn => "Don't use this method." do |_self|
      false
    end
  end

  class Device
    extend GLib::Deprecatable
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

  module Keyval
    extend GLib::Deprecatable
    constants.each do |key|
      define_deprecated_const key.to_s.sub(/^GDK_KEY_/, 'GDK_'), [self, key].join('::')
    end
  end

  class Window
    extend GLib::Deprecatable
    define_deprecated_method :clear, :warn => "Don't use this method."
    define_deprecated_method :clear_area, :warn => "Don't use this method."
    define_deprecated_method :internal_paint_info, :raise => "Don't use this method."
    define_deprecated_method :shape_combine_mask, :warn => "Don't use this method."
    define_deprecated_method :input_shape_combine_mask, :warn => "Don't use this method."
    define_deprecated_method :set_back_pixmap, :warn => "Don't use this method."
  end
end

