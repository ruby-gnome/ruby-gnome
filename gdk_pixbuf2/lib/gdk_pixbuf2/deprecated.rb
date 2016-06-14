# Copyright (C) 2016  Ruby-GNOME2 Project Team
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

module GdkPixbuf
  class Pixbuf
    extend GLib::Deprecatable

    define_deprecated_enums "Colorspace", "COLORSPACE"
    define_deprecated_enums "InterpType", "INTERP"
    define_deprecated_enums "PixbufRotation", "ROTATE"

    class << self
      def new(*args, &block)
        if args[0].is_a?(Pixbuf)
          message = "#{caller[0]}: #{self}.new(pixbuf, ...) is deprecated. "
          message << "Use pixbuf.new_subpixbuf(...)  instead."
          warn(message)
          args[0].new_subpixbuf(*args[1..-1])
        elsif args.size == 1 and args[0].is_a?(Hash)
          options = args[0]
          src_pixbuf = options[:src_pixbuf]
          if src_pixbuf
            message = "#{caller[0]}: "
            message << "#{self}.new(:src_pixbuf => pixbuf, ...) is deprecated. "
            message << "Use pixbuf.new_subpixbuf(...)  instead."
            warn(message)
            src_pixbuf.new_subpixbuf(options[:src_x],
                                     options[:src_y],
                                     options[:width],
                                     options[:height])
          else
            super
          end
        else
          super
        end
      end
    end
  end

  class PixbufFormat
    extend GLib::Deprecatable

    define_deprecated_method "domain", :raise => "Don't use this method anymore."
  end

  class Pixdata
    extend GLib::Deprecatable

    unless respond_to?(:from_pixbuf)
      define_deprecated_singleton_method :from_pixbuf,
                                         :raise => "Use Gio::Resource instead"
    end
  end
end

module Gdk
  extend GLib::Deprecatable

  define_deprecated_const :Pixbuf, "GdkPixbuf::Pixbuf"
  define_deprecated_const :PixbufAnimation, "GdkPixbuf::PixbufAnimation"
  define_deprecated_const :PixbufLoader, "GdkPixbuf::PixbufLoader"
  define_deprecated_const :PixbufSimpleAnim, "GdkPixbuf::PixbufSimpleAnim"
  define_deprecated_const :Pixdata, "GdkPixbuf::Pixdata"
end
