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

module Poppler
  class Page
    alias_method :text_layout_raw, :text_layout
    def text_layout
      success, rectangles = text_layout_raw
      if success
        rectangles
      else
        nil
      end
    end

    alias_method :image_mapping_raw, :image_mapping
    def image_mapping
      mappings = image_mapping_raw
      mappings.each do |mapping|
        mapping.page = self
      end
      mappings
    end

    def get_text(options={})
      area = options[:area]
      style = options[:style]
      if area
        if style
          get_selected_text(style, area)
        else
          get_text_for_area(area)
        end
      else
        text
      end
    end

    alias_method :find_text_raw, :find_text
    def find_text(text, options=nil)
      if options.nil?
        find_text_raw(text)
      else
        find_text_with_options(text, options)
      end
    end

    alias_method :thumbnail_size_raw, :thumbnail_size
    def thumbnail_size
      have_thumnbial, width, height = thumbnail_size_raw
      if have_thumnbial
        [width, height]
      else
        nil
      end
    end
  end
end
