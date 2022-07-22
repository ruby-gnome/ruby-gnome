# Copyright (C) 2011-2022  Ruby-GNOME Project Team
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
require "fileutils"

require "gtk4"

module GtkTestUtils
  private
  def only_gtk_version(major, minor, micro=nil)
    micro ||= 0
    unless Gtk::Version.or_later?(major, minor, micro)
      omit("Require GTK+ >= #{major}.#{minor}.#{micro}")
    end
  end

  def window_system_type_name
    Gdk::Display.default.class.gtype.name
  end

  module WindowSystemTypeNames
    X11 = "GdkX11Display"
    WINDOWS = "GdkWin32Display"
  end

  def x11?
    window_system_type_name == WindowSystemTypeNames::X11
  end

  def fixture_path(*components)
    File.expand_path(File.join(*components),
                     ENV["GTK4_FIXTURE_DIR"] || File.join("test", "fixture"))
  end

  def suppress_warning
    verbose, $VERBOSE = $VERBOSE, nil
    begin
      yield
    ensure
      $VERBOSE = verbose
    end
  end
end
