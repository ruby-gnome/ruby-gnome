# Copyright (C) 2015  Ruby-GNOME2 Project Team
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
  module Stock
    class << self
      alias_method :add_raw, :add
      def add(stock_id, label, *rest)
        case rest.size
        when 0
          options = {}
        when 1
          if rest[0].is_a?(Hash)
            options = rest[0]
          else
            options = {:modifier => rest[0]}
          end
        else
          options = {
            :modifier           => rest[0],
            :key_value          => rest[1],
            :translation_domain => rest[2],
          }
        end

        item = StockItem.new

        stock_id = stock_id.to_s if stock_id.is_a?(Symbol)
        item.stock_id = stock_id

        item.label = label

        modifier = options[:modifier]
        item.modifier = modifier if modifier

        key_value = options[:key_value]
        item.keyval = key_value if key_value

        translation_domain = options[:translation_domain]
        item.translation_domain = translation_domain if translation_domain

        add_raw([item])
      end

      alias_method :lookup_raw, :lookup
      def lookup(stock_id)
        stock_id = stock_id.to_s if stock_id.is_a?(Symbol)
        found, item = lookup_raw(stock_id)
        if found
          item
        else
          nil
        end
      end
    end
  end
end
