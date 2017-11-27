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

module Pango
  class << self
    alias_method :parse_markup_raw, :parse_markup
    def parse_markup(markup_text, accel_marker=nil)
      accel_marker ||= 0
      success, attr_list, text, accel_char =
        parse_markup_raw(markup_text,
                         markup_text.bytesize,
                         accel_marker)
      return nil unless success

      accel_char = nil if accel_char.empty?
      [attr_list, text, accel_char]
    end
  end
end
