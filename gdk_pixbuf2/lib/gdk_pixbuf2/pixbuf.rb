module GdkPixbuf
  class Pixbuf
    alias_method :initialize_raw, :initialize

    def initialize(*args)
      case args.size
      when 1
        if args[0].class == Hash
          initialize_with_hash(args[0])
        elsif args[0].class == String
          puts deprecated_usage_message
          initialize_raw(args[0])
        elsif args[0].class == Array
          puts deprecated_usage_message
          initialize_new_from_xpm_data(args[0])
        else
          puts "Wrong type of arguments"
          puts deprecated_usage_message
        end
      when 2
        # initialize_from_inline_data is deprecated
      when 3
        puts deprecated_usage_message
        initialize_new_from_file_at_size(*args)
      when 4
        puts deprecated_usage_message
        initialize_new_from_file_at_scale(*args)
      when 5
        if args[0].class == GdkPixbuf::Pixbuf
          puts deprecated_usage_message
          initialize_new_from_subpixbuf(*args)
        elsif args[0].class == GdkPixbuf::Colorspace # No other value
          puts deprecated_usage_message
          initialize_new(*args)
        else
          puts "Wrong type of 1st argument, must be a Pixbuf or Colorspace"
        end
      when 7
        puts deprecated_usage_message
        initialize_new_from_data(*args)
      else
        puts "Wrong number of arguments"
        puts deprecated_usage_message
      end
    end

    def deprecated_usage_message
      "Please use the hash form for the arguments :colorspace, :has_alpha, " +
      ":bits_per_sample, :row_stride, :src_x, :src_y, :src_pixbuf, :data, " +
      ":bytes, :xpm, :file, :stream, :resource, :width, :height, :scale, " +
      ":preserve_aspect_ration"
    end

    def initialize_with_hash(options)
      colorspace = options[:colorspace] || GdkPixbuf::Colorspace::RGB
      has_alpha = options[:has_alpha] || false
      bits_per_sample = options[:bits_per_sample] || 8
      row_stride = options[:row_stride] || 0
      src_x = options[:src_x] || 0
      src_y = options[:src_y] || 0
      src_pixbuf = options[:src_pixbuf] || nil
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
      preserve_aspect_ratio = options[:preserve_aspect_ratio] || true


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
        initialize_new_from_data(colorspace, has_alpha, bits_per_sample,
                                 width, height, row_stride)
      elsif bytes && size
         initialize_new_from_bytes(colorspace, has_alpha, bits_per_sample,
                                   width, height, row_stride)

      elsif xpm
        initialize_new_from_xpm_data(xpm)
      elsif src_pixbuf && size
        initialize_new_from_subpixbuf(src_pixbuf, src_x, src_y, width, height)
      elsif size
        initialize_raw(colorspace, has_alpha, bits_per_sample, width, height)
      else
        puts "Please provide a width and an height"
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
  end
end
