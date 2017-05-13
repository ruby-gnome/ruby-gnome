# Copyright (C) 2017  Ruby-GNOME2 Project Team
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

require "gobject-introspection"
require "glib2"
require "gio2"
require "gdk_pixbuf2"
require "cairo"

base_dir = Pathname.new(__FILE__).dirname.dirname.expand_path
vendor_dir = base_dir + "vendor" + "local"
vendor_bin_dir = vendor_dir + "bin"
GLib.prepend_dll_path(vendor_bin_dir)
vendor_girepository_dir = vendor_dir + "lib" + "girepository-1.0"
GObjectIntrospection.prepend_typelib_path(vendor_girepository_dir)

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

require "rsvg2/loader"

module Rsvg
  LOG_DOMAIN = "librsvg"
  GLib::Log.set_log_domain(LOG_DOMAIN)

  loader = Loader.new(self)
  loader.load("Rsvg")
end

RSVG = Rsvg # for backward compatibility
