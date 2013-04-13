=begin
  gtksourceview2.rb

  Copyright (c) 2008-2011 Ruby-GNOME2 Project Team
  This program is licensed under the same licence as Ruby-GNOME2.
=end

require 'gtk2'

base_dir = Pathname.new(__FILE__).dirname.dirname.expand_path
vendor_dir = base_dir + "vendor" + "local"
vendor_bin_dir = vendor_dir + "bin"
GLib.prepend_dll_path(vendor_bin_dir)
begin
  major, minor, micro, = RUBY_VERSION.split(/\./)
  require "#{major}.#{minor}/gtksourceview2.so"
rescue LoadError
  require "gtksourceview2.so"
end
