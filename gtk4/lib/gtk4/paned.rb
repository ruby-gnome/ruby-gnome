# Copyright (C) 2015-2022  Ruby-GNOME Project Team
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
  class Paned
    alias_method :set_start_child_raw, :set_start_child
    def set_start_child(child, options=nil)
      set_start_child_raw(child)
      return self unless options

      resize = options[:resize]
      set_resize_start_child(resize) unless resize.nil?
      shrink = options[:shrink]
      set_shrink_start_child(shrink) unless shrink.nil?
      self
    end

    alias_method :set_end_child_raw, :set_end_child
    def set_end_child(child, options=nil)
      set_end_child_raw(child)
      return self unless options

      resize = options[:resize]
      set_resize_end_child(resize) unless resize.nil?
      shrink = options[:shrink]
      set_shrink_end_child(shrink) unless shrink.nil?
      self
    end
  end
end
