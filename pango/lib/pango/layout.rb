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
  class Layout
    alias_method :set_font_description_raw, :set_font_description
    def set_font_description(description)
      unless description.is_a?(FontDescription)
        description = FontDescription.new(description)
      end
      set_font_description_raw(description)
    end
    remove_method :font_description=
    alias_method :font_description=, :set_font_description

    alias_method :set_text_raw, :set_text
    def set_text(text)
      set_text_raw(text, text.bytesize)
    end
    alias_method :text=, :set_text

    alias_method :set_markup_raw, :set_markup
    def set_markup(markup)
      set_markup_raw(markup, markup.bytesize)
    end
    alias_method :markup=, :set_markup
  end
end
