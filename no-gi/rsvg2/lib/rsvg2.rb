# Copyright (C) 2004-2015  Ruby-GNOME2 Project Team
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

require "glib2"
require "gdk_pixbuf2"
require "cairo"

base_dir = Pathname.new(__FILE__).dirname.dirname.expand_path
vendor_dir = base_dir + "vendor" + "local"
vendor_bin_dir = vendor_dir + "bin"
GLib.prepend_dll_path(vendor_bin_dir)

if vendor_dir.exist?
  require "pango"

  gdk_pixbuf2_spec = Gem.loaded_specs["gdk_pixbuf2"]
  gdk_pixbuf2_base_dir = Pathname.new(gdk_pixbuf2_spec.gem_dir)
  gdk_pixbuf2_module_dir =
    gdk_pixbuf2_base_dir + "vendor" + "local" + "lib" +
    "gdk-pixbuf-2.0" + "2.10.0"
  gdk_pixbuf2_loaders_cache_path = gdk_pixbuf2_module_dir + "loaders.cache"
  need_loaders_cache_update = false
  if gdk_pixbuf2_loaders_cache_path.exist?
    gdk_pixbuf2_loaders_cache_path.open do |cache|
      need_loaders_cache_update = cache.each_line.none? do |line|
        /\A"svg"/ === line
      end
    end
  else
    need_loaders_cache_update = true
  end
  if need_loaders_cache_update
    pid = spawn("gdk-pixbuf-query-loaders.exe",
                :out => [gdk_pixbuf2_loaders_cache_path.to_s, "w"])
    Process.waitpid(pid)
  end
end

begin
  major, minor, = RUBY_VERSION.split(/\./)
  require "#{major}.#{minor}/rsvg2.so"
rescue LoadError
  require "rsvg2.so"
end

module RSVG
  LOG_DOMAIN = "librsvg"

  class << self
    def cairo_available?
      true
    end
  end

  class Handle
    class << self
      # For backward compatibility
      def new_from_data(data)
        new(:data => data)
      end

      # For backward compatibility
      def new_from_file(file_name, options={})
        new(options.merge(:file_name => file_name))
      end
    end
  end

  module Version
    MAJOR, MINOR, MICRO = BUILD_VERSION
    STRING = "#{MAJOR}.#{MINOR}.#{MICRO}"

    class << self
      def or_later?(major, minor, micro=nil)
        micro || 0
        version = [
          MAJOR,
          MINOR,
          MICRO,
        ]
        (version <=> [major, minor, micro]) >= 0
      end
    end
  end

end

module Cairo
  class Context
    def render_rsvg_handle(handle, *args, &block)
      handle.render_cairo(self, *args, &block)
    end
  end
end

GLib::Log.set_log_domain(RSVG::LOG_DOMAIN)
