# Copyright (C) 2011-2015  Ruby-GNOME2 Project Team
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

require "tempfile"

module GtkTestUtils
  private
  def only_gtk_version(major, minor, micro=nil)
    micro ||= 0
    unless Gtk::Version.or_later?(major, minor, micro)
      omit("Require GTK+ >= #{major}.#{minor}.#{micro}")
    end
  end

  def window_system_type_name
    Gdk::Screen.default.class.gtype.name
  end

  def csd_supported?
    screen = Gdk::Screen.default
    case window_system_type_name
    when "GdkX11Screen"
      screen.composited? and screen.rgba_visual
    when "GdkWin32Screen"
      screen.rgba_visual
    else
      true
    end
  end
end
