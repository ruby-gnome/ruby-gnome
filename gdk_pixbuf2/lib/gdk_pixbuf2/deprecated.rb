# Copyright (C) 2016-2017  Ruby-GNOME2 Project Team
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
          message << "Use pixbuf.subpixbuf(...)  instead."
          warn(message)
          args[0].subpixbuf(*args[1..-1])
        elsif args.size == 1 and args[0].is_a?(Hash)
          options = args[0]
          src_pixbuf = options[:src_pixbuf]
          if src_pixbuf
            message = "#{caller[0]}: "
            message << "#{self}.new(:src_pixbuf => pixbuf, ...) is deprecated. "
            message << "Use pixbuf.subpixbuf(...)  instead."
            warn(message)
            src_pixbuf.subpixbuf(options[:src_x],
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

    define_deprecated_method :new_subpixbuf, :subpixbuf

    define_deprecated_method :save_to_buffer, :save

    define_deprecated_method_by_hash_args :composite,
                                          "dest_width, " +
                                            "dest_height, " +
                                            "interpolation_type, " +
                                            "overall_alpha, " +
                                            "check_size, " +
                                            "color1, " +
                                            "color2",
                                          ":dest_width => dest_width, " +
                                            ":dest_height => dest_height, " +
                                            ":interpolation_type => interpolation_type, " +
                                            ":overall_alpha => overall_alpha, " +
                                            ":check_size => check_size, " +
                                            ":color1 => color1, " +
                                            ":color2 => color2" do |*args|
      [
        :dest_width => args[1],
        :dest_height => args[2],
        :interpolation_type => args[3],
        :overall_alpha => args[4],
        :check_size => args[5],
        :color1 => args[6],
        :color2 => args[7],
      ]
    end

    define_deprecated_method_by_hash_args :composite!,
                                          "source, " +
                                            "dest_x, " +
                                            "dest_y, " +
                                            "dest_width, " +
                                            "dest_height, " +
                                            "offset_x, " +
                                            "offset_y, " +
                                            "scale_x, " +
                                            "scale_y, " +
                                            "interpolation_type, " +
                                            "overall_alpha, " +
                                            "check_x=0, " +
                                            "check_y=0, " +
                                            "check_size=nil, " +
                                            "color1=0x999999, " +
                                            "color2=0xdddddd",
                                          "source, " +
                                            ":dest_x => dest_x, " +
                                            ":dest_y => dest_y, " +
                                            ":dest_width => dest_width, " +
                                            ":dest_height => dest_height, " +
                                            ":offset_x => offset_x, " +
                                            ":offset_y => offset_y, " +
                                            ":scale_x => scale_x, " +
                                            ":scale_y => scale_y, " +
                                            ":interpolation_type => interpolation_type, " +
                                            ":overall_alpha => overall_alpha, " +
                                            ":check_x => check_x, " +
                                            ":check_y => check_y, " +
                                            ":check_size => check_size, " +
                                            ":color1 => color1, " +
                                            ":color2 => color2",
                                          1 do |*args|
      [
        args[1],
        :dest_x => args[2],
        :dest_y => args[3],
        :dest_width => args[4],
        :dest_height => args[5],
        :offset_x => args[6],
        :offset_y => args[7],
        :scale_x => args[8],
        :scale_y => args[9],
        :interpolation_type => args[10],
        :overall_alpha => args[11],
        :check_x => args[12],
        :check_y => args[13],
        :check_size => args[14],
        :color1 => args[15],
        :color2 => args[16],
      ]
    end
  end

  class PixbufFormat
    extend GLib::Deprecatable

    define_deprecated_method "domain", :raise => "Don't use this method anymore."
    define_deprecated_method "signature", :raise => "Don't use this method anymore."
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
  define_deprecated_const :PixbufError, "GdkPixbuf::PixbufError"
end
