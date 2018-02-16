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

module Gtk
  class TextTag
    alias_method :set_weight_raw, :set_weight
    def set_weight(weight)
      case weight
      when Symbol, String
        weight = Pango::Weight.new(weight).to_i
      end
      set_weight_raw(weight)
    end
    undef_method :weight=
    alias_method :weight=, :set_weight

    alias_method :set_scale_raw, :set_scale
    def set_scale(scale)
      case scale
      when Symbol, String
        scale = Pango::Scale.const_get(scale.to_s.upcase)
      end
      set_scale_raw(scale)
    end
    undef_method :scale=
    alias_method :scale=, :set_scale
  end
end
