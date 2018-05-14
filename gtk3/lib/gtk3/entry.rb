# Copyright (C) 2018  Ruby-GNOME2 Project Team
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

module Gtk
  class Entry
    # TODO: It's workaround for a GTK+ problem:
    # It should be removed when GTK+ fix the problem.
    # https://github.com/ruby-gnome2/ruby-gnome2/issues/1176
    alias_method :set_attributes_raw, :set_attributes
    def set_attributes(attrs)
      attrs ||= Pango::AttrList.new
      set_attributes_raw(attrs)
    end
    remove_method :attributes=
    alias_method :attributes=, :set_attributes
  end
end
