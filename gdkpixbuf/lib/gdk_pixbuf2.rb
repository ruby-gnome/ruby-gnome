require 'glib2'
require 'gdk_pixbuf2.so'

module Gdk
  class PixbufLoader
    def self.open(type = nil, mime_type = false)
      loader = Gdk::PixbufLoader.new(type, mime_type)
      if block_given?
        begin
          yield(loader)
        ensure
          loader.close
        end
      end
      loader
    end
  end
end
