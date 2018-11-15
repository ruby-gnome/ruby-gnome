# Copyright (C) 2016-2018  Ruby-GNOME2 Project Team
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
        message << "width, height) is deprecated. "
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
        message << "bits_per_sample, width, height) is deprecated. "
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
      row_stride = options[:row_stride]
      data = options[:data]
      bytes = options[:bytes]
      xpm = options[:xpm]
      file = options[:file]
      # stream = options[:stream]
      resource = options[:resource]
      width = options[:width]
      height = options[:height]
      size = true if width && height
      scale = options[:scale]
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
        initialize_new_from_resource_at_scale(resource, width, height,
                                              preserve_aspect_ratio)
      elsif resource && !scale
        initialize_new_from_resource(resource)
      elsif data && size
        data = data.pack("C*") if data.is_a?(Array)
        row_stride ||= data.bytesize / height
        initialize_new_from_data(data,
                                 colorspace,
                                 has_alpha,
                                 bits_per_sample,
                                 width,
                                 height,
                                 row_stride)
        @data = data
      elsif bytes && size
        row_stride ||= bytes.size / height
        initialize_new_from_bytes(bytes,
                                  colorspace,
                                  has_alpha,
                                  bits_per_sample,
                                  width,
                                  height,
                                  row_stride)
        @bytes = bytes
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

    alias_method :row_stride, :rowstride

    def dup
      copy
    end

    if method_defined?(:pixels_with_length)
      remove_method :pixels
      alias_method :pixels, :pixels_with_length
      remove_method :pixels_with_length
    end

    def fill!(pixel)
      fill(pixel)
    end

    def rotate(angle)
      rotate_simple(angle)
    end

    alias_method :saturate_and_pixelate_raw, :saturate_and_pixelate
    def saturate_and_pixelate(saturation, pixelate)
      dest = dup
      saturate_and_pixelate_raw(dest, saturation, pixelate)
      dest
    end

    # @overload save(type, options={})
    #
    #   Save as `type` format.
    #
    #   @param type [String] The format to be saved. `"jpeg"`,
    #     `"png"`, `"ico"` and `"bmp"` are available by default.
    #
    #   @param options [Hash<Symbol, String>] The options for
    #     saving. Key-value pairs except `:filename` key are passed to
    #     save logic. Available keys are depended on format. For
    #     example, `:quality => "100"` is available in `"jpeg"`
    #     format.
    #   @option options [String] :filename The filename to be outputted.
    #
    #   @return [String, void] The saved data. If you specify
    #     `:filename` option, it returns nothing.
    #
    # @overload save(filename, type, options={})
    #
    #   Save to `filename` as `type` format.
    #
    #   @return [void]
    #
    #   @deprecated since 3.1.1. Use `save(type, :filename =>
    #     filename)` instead.
    def save(*args)
      case args.size
      when 1
        arg1 = args[0]
        if arg1.respond_to?(:to_path)
          filename = arg1.to_path
        elsif arg1.is_a?(String) and arg1.include?(".")
          filename = args[0]
        else
          filename = nil
          type = arg1
        end
        if filename
          type = File.extname(filename).gsub(/\A\./, "").downcase
          case type
          when "jpg"
            type = "jpeg"
          end
        end
        options = {}
      when 2
        if args.last.is_a?(Hash)
          type, options = args
          if options.key?(:filename)
            options = options.dup
            filename = options.delete(:filename)
          else
            filename = nil
          end
        else
          filename, type = args
          options = {}
        end
      when 3
        filename, type, options = args
      else
        message = "wrong number of arguments (given #{args.size}, expected 1..3)"
        raise ArgumentError, message
      end

      keys = []
      values = []
      options.each do |key, value|
        key = key.to_s if key.is_a?(Symbol)
        keys << key
        values << value
      end
      if filename
        savev(filename, type, keys, values)
      else
        _, data = save_to_bufferv(type, keys, values)
        data.pack("C*")
      end
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

    alias_method :composite_raw, :composite
    def composite(options)
      destination = options[:destination] || options[:dest]
      dest_x = options[:destination_x] || options[:dest_x] || 0
      dest_y = options[:destination_y] || options[:dest_y] || 0
      dest_width = options[:destination_width] || options[:dest_width]
      dest_height = options[:destination_height] || options[:dest_height]

      destination ||= Pixbuf.new(:colorspace => colorspace,
                                 :has_alpha => has_alpha?,
                                 :bits_per_sample => bits_per_sample,
                                 :width => dest_x + dest_width,
                                 :height => dest_y + dest_height)
      destination.composite!(self, options)
      destination
    end

    def composite!(source, options)
      dest_x = options[:destination_x] || options[:dest_x] || 0
      dest_y = options[:destination_y] || options[:dest_y] || 0
      dest_width = options[:destination_width] || options[:dest_width]
      dest_height = options[:destination_height] || options[:dest_height]
      offset_x = options[:offset_x] || 0.0
      offset_y = options[:offset_y] || 0.0
      scale_x = options[:scale_x] || (dest_width / source.width.to_f)
      scale_y = options[:scale_y] || (dest_height / source.height.to_f)
      interpolation_type = options[:interpolation_type] ||
        options[:interp_type] ||
        :bilinear
      overall_alpha = options[:overall_alpha] || 255
      check_x = options[:check_x] || 0
      check_y = options[:check_y] || 0
      check_size = options[:check_size]
      color1 = options[:color1] || 0x999999
      color2 = options[:color2] || 0xdddddd

      if check_size
        source.composite_color(self,
                               dest_x,
                               dest_y,
                               dest_width,
                               dest_height,
                               offset_x,
                               offset_y,
                               scale_x,
                               scale_y,
                               interpolation_type,
                               overall_alpha,
                               check_x,
                               check_y,
                               check_size,
                               color1,
                               color2)
      else
        source.composite_raw(self,
                             dest_x,
                             dest_y,
                             dest_width,
                             dest_height,
                             offset_x,
                             offset_y,
                             scale_x,
                             scale_y,
                             interpolation_type,
                             overall_alpha)
      end
    end
  end
end
