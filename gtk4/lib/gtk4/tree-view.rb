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
  class TreeView
    alias_method :get_path_at_pos_raw, :get_path_at_pos
    def get_path_at_pos(x, y)
      found, *info = get_path_at_pos_raw(x, y)
      if found
        info
      else
        nil
      end
    end

    alias_method :enable_model_drag_source_raw, :enable_model_drag_source
    def enable_model_drag_source(flags, targets, actions)
      targets = ensure_drag_targets(targets)
      enable_model_drag_source_raw(flags, targets, actions)
    end

    alias_method :enable_model_drag_dest_raw, :enable_model_drag_dest
    def enable_model_drag_dest(targets, actions)
      targets = ensure_drag_targets(targets)
      enable_model_drag_dest_raw(targets, actions)
    end

    alias_method :insert_column_raw, :insert_column
    def insert_column(*args, &block)
      case args.size
      when 2
        column, position = args
        insert_column_raw(column, position)
      when 3
        position, title, cell = args
        insert_column_with_data_func(position, title, cell, &block)
      when 4
        position, title, cell, attributes = args
        column = TreeViewColumn.new
        column.sizing = :fixed if fixed_height_mode?
        column.title = title
        column.pack_start(cell, true)
        attributes.each do |name, column_id|
          column.add_attribute(cell, name, column_id)
        end
        insert_column_raw(column, position)
      else
        raise ArgumentError, "wrong number of arguments (#{args.size} for 2..4)"
      end
    end

    alias_method :expand_row_raw, :expand_row
    def expand_row(path, options={})
      if options == true or options == false
        open_all = options
      else
        open_all = options[:open_all]
        open_all = true if open_all.nil?
      end

      expand_row_raw(path, open_all)
    end

    private
    def create_signal_handler(signal_name, callback)
      case signal_name
      when "row-collapsed", "row-expanded"
        lambda do |tree_view, iter, path, *extra_args|
          iter.model = tree_view.model
          callback.call(tree_view, iter, path, *extra_args)
        end
      else
        super
      end
    end
  end
end
