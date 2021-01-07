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

require "tempfile"

require "gdk_pixbuf2"

module GdkPixbufTestUtils
  private
  def only_version(major, minor, micro=nil)
    micro ||= 0
    unless GdkPixbuf::Version.or_later?(major, minor, micro)
      omit("Require GdkPixbuf >= #{major}.#{minor}.#{micro}")
    end
  end

  def fixture_path(*components)
    File.join(File.dirname(__FILE__), "fixture", *components)
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
