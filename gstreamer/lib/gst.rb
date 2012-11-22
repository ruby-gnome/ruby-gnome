require 'rational'
require 'glib2'

base_dir = Pathname.new(__FILE__).dirname.dirname.expand_path
vendor_dir = base_dir + "vendor" + "local"
vendor_bin_dir = vendor_dir + "bin"
GLib.prepend_environment_path(vendor_bin_dir)
if vendor_bin_dir.exist?
  require 'pango'
  require 'gdk_pixbuf2'
end
begin
  major, minor, _ = RUBY_VERSION.split(/\./)
  require "#{major}.#{minor}/gstreamer.so"
rescue LoadError
  require "gstreamer.so"
end

argv_without_help_options = ARGV.reject do |option|
  /\A-h|--help\z/ =~ option
end
Gst.init(argv_without_help_options)

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
