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
end

module Cairo
  class Context
    def render_rsvg_handle(handle, *args, &block)
      handle.render_cairo(self, *args, &block)
    end
  end
end

GLib::Log.set_log_domain(RSVG::LOG_DOMAIN)
