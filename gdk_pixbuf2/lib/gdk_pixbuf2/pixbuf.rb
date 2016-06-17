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
    alias_method :initialize_raw, :initialize

    def initialize(*args)
      case args.size
      when 1
        case args[0]
        when Hash
          initialize_with_hash(args[0])
        when String
          message = "#{caller[0]}: #{self.class}.new(path) is deprecated. "
          message << "Use #{self.class}.new(:file => path) instead."
          warn message
          initialize_raw(args[0])
        when Array
          message = "#{caller[0]}: #{self.class}.new(xpm) is deprecated. "
          message << "Use #{self.class}.new(:xpm => xpm) instead."
          warn message
          initialize_new_from_xpm_data(args[0])
        else
          raise ArgumentError, "must be options: #{args[0].inspect}"
        end
      when 2
        message = "#{caller[0]}: "
        message << "#{self.class}.new(data, copy_pixels) is deprecated. "
        message << "Use Gio::Resource instead."
        warn message
        initialize_from_inline(*args)
      when 3
        message = "#{caller[0]}: "
        message << "#{self.class}.new(path, width, height) is deprecated. "
        message << "Use #{self.class}.new(:file => path, :width => width, "
        message << ":height => height) instead."
        warn message
        initialize_new_from_file_at_size(*args)
      when 4
        message = "#{caller[0]}: "
        message << "#{self.class}.new(path, width, height, "
        message << "preserve_aspect_ratio) is deprecated. "
        message << "Use #{self.class}.new(:file => path, :width => width, "
        message << ":height => height, "
        message << ":preserve_aspect_ratio => preserve_aspect_ratio) instead."
        warn message
        initialize_new_from_file_at_scale(*args)
      when 5
        message = "#{caller[0]}: "
        message << "#{self.class}.new(colorspace, has_alpha, bits_per_sample, "
        message << "width, height) is deprecated."
        message << "Use #{self.class}.new(:colorspace => colorspace, "
        message << ":has_alpha => has_alpha, "
        message << ":bits_per_sample => bits_per_sample, "
        message << ":width => width, "
        message << ":height => height) instead."
        warn message
        initialize_new(*args)
      when 7
        message = "#{caller[0]}: "
        message << "#{self.class}.new(data, colorspace, has_alpha, "
        message << "bits_per_sample, width, height) is deprecated."
        message << "Use #{self.class}.new(:data => data, "
        message << ":colorspace => colorspace, "
        message << ":has_alpha => has_alpha, "
        message << ":bits_per_sample => bits_per_sample, "
        message << ":width => width, "
        message << ":height => height) instead."
        warn message
        initialize_new_from_data(*args)
      else
        super
      end
    end

    def initialize_with_hash(options)
      colorspace = options[:colorspace] || GdkPixbuf::Colorspace::RGB
      has_alpha = options[:has_alpha]
      has_alpha = false if has_alpha.nil?
      bits_per_sample = options[:bits_per_sample] || 8
      row_stride = options[:row_stride] || 0
      data = options[:data] || nil
      bytes = options[:bytes] || nil
      xpm = options[:xpm] || nil
      file = options[:file] || nil
      stream = options[:stream] || nil
      resource = options[:resource] || nil
      width = options[:width] || nil
      height = options[:height] || nil
      size = true if width && height
      scale = options[:scale] || nil
      preserve_aspect_ratio = options[:preserve_aspect_ratio]
      preserve_aspect_ratio = true if preserve_aspect_ratio.nil?

      if file && size && scale
        initialize_new_from_file_at_scale(file, width, height,
                                          preserve_aspect_ratio)
      elsif file && size && !scale
        initialize_new_from_file_at_size(file, width, height)
      elsif file && !size
        initialize_new_from_file(file)
      elsif resource && size && scale
        initialize_new_from_resource_at_scale(resource, width, height)
      elsif resource && !scale
        initialize_new_from_resource(resource)
      elsif data && size
        initialize_new_from_data(data, colorspace, has_alpha, bits_per_sample,
                                 width, height, row_stride)
      elsif bytes && size
         initialize_new_from_bytes(bytes, colorspace, has_alpha, bits_per_sample,
                                   width, height, row_stride)

      elsif xpm
        initialize_new_from_xpm_data(xpm)
      elsif size
        initialize_raw(colorspace, has_alpha, bits_per_sample, width, height)
      else
        message =
          "must specify :file, :resource, :data, :bytes, :xpm, :width or :height"
        raise ArgumentError, message
      end
      # https://developer.gnome.org/gdk-pixbuf/2.33/gdk-pixbuf-Image-Data-in-Memory.html
      #	gdk_pixbuf_new                                  done
      #	gdk_pixbuf_new_from_bytes                       done
      #	gdk_pixbuf_new_from_data                        done
      #	gdk_pixbuf_new_from_xpm_data                    done
      #	gdk_pixbuf_new_subpixbuf
      #	https://developer.gnome.org/gdk-pixbuf/2.33/gdk-pixbuf-File-Loading.html
      #	gdk_pixbuf_new_from_file                        done
      # gdk_pixbuf_new_from_file_at_size                done
      #	gdk_pixbuf_new_from_file_at_scale               done
      #	gdk_pixbuf_get_file_info
      #	gdk_pixbuf_get_file_info_async
      # gdk_pixbuf_get_file_info_finish
      #	gdk_pixbuf_new_from_resource                    done
      #	gdk_pixbuf_new_from_resource_at_scale           done
      #	gdk_pixbuf_new_from_stream
      # gdk_pixbuf_new_from_stream_async
      #	gdk_pixbuf_new_from_stream_finish
      #	gdk_pixbuf_new_from_stream_at_scale
      #	gdk_pixbuf_new_from_stream_at_scale_async
    end
    private :initialize_with_hash

    def dup
      copy
    end

    # TODO: test
    def fill!(pixel)
      fill(pixel)
    end

    # TODO: test
    def rotate(angle)
      rotate_simple(angle)
    end

    # TODO: test
    alias_method :saturate_and_pixelate_raw, :saturate_and_pixelate
    def saturate_and_pixelate(saturation, pixelate)
      dest = dup
      saturate_and_pixelate_raw(dest, saturation, pixelate)
      dest
    end

    # TODO: test
    # TODO: Improve API by Hash
    def save(filename, type, options={})
      keys = []
      values = []
      options.each do |key, value|
        keys << key
        values << value
      end
      savev(filename, type, keys, values)
    end

    alias_method :scale_raw, :scale
    # TODO: test
    # TODO: Improve API by Hash
    def scale(*args)
      case args.size
      when 2, 3
        width, height, interp_type = args
        interp_type ||= :bilinear
        scale_simple(width, height, interp_type)
      else
        scale_raw(*args)
      end
    end

    # TODO: test
    # TODO: Improve API by Hash
    def scale!(source, *args)
      if args.size == 8
        args << :bilinear
      end
      source.scale_raw(self, *args)
    end
  end
end
