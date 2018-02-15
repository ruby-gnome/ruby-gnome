# Copyright (C) 2014-2016  Ruby-GNOME2 Project Team
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
  class TreeViewColumn
    alias_method :initialize_raw, :initialize
    def initialize(*args)
      if args.size == 1 and args[0].is_a?(Hash)
        options = args[0].dup
        area = options.delete(:area)
        renderer = options.delete(:renderer)
        title = options.delete(:title)
        attributes = options.delete(:attributes)
        unless options.empty?
          names = options.keys.inspect
          available_names = [:area, :renderer, :title, :attributes].inspect
          message =
            "unknown option(s): #{names}: available options: #{available_names}"
          raise ArgumentError, message
        end
      else
        area = nil
        title, renderer, attributes = args
      end
      attributes ||= {}

      if area
        initialize_new_with_area(area)
      else
        initialize_raw
      end

      set_title(title) if title
      if renderer
        pack_start(renderer, true)
        attributes.each_entry do |key, value|
          add_attribute(renderer, key, value)
        end
      end
    end

    alias_method :add_attribute_raw, :add_attribute
    def add_attribute(renderer, key, value)
      key = key.to_s if key.is_a?(Symbol)
      add_attribute_raw(renderer, key, value)
    end
  end
end
