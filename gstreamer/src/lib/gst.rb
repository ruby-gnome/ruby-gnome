require 'glib2'
require 'gst.so'

Gst.init

module Gst
  class Plugin
    def each_feature(&block)
      Registry.default.each_feature(name, &block)
    end
  end

  class Structure
    def inspect
      super.gsub(/>\z/, ": #{to_s}>")
    end
  end
end
