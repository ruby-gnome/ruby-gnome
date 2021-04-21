# Copyright (C) 2015-2021  Ruby-GNOME Project Team
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

require "pathname"

require "test-unit"

require "glib2"

module GLibTestUtils
  private
  def only_glib_version(major, minor, micro)
    unless GLib.check_version?(major, minor, micro)
      omit("Require GLib >= #{major}.#{minor}.#{micro}")
    end
  end

  def only_windows
    omit("Only for Windows platform") unless GLib.os_win32?
  end

  def only_not_windows
    omit("Not for for Windows platform") if GLib.os_win32?
  end

  def normalize_path(path)
    return path unless File::ALT_SEPARATOR
    path.gsub(File::ALT_SEPARATOR, File::SEPARATOR)
  end
end
