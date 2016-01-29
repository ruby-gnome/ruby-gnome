# Copyright (c) 2015 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
=  Tree View/Filter Model

 This example demonstrates how GtkTreeModelFilter can be used not
 just to show a subset of the rows, but also to compute columns
 that are not actually present in the underlying model.
=end
module FiltermodelDemo
  WIDTH_COLUMN = 0
  HEIGHT_COLUMN = 1
  AREA_COLUMN = 2
  SQUARE_COLUMN = 3

  def self.run_demo(main_window)
    builder = Gtk::Builder.new(:resource => "/filtermodel/filtermodel.ui")
    builder.connect_signals {}

    window = builder["window1"]
    window.screen = main_window.screen

    column = builder["treeviewcolumn1"]
    cell = builder["cellrenderertext1"]
    column.set_cell_data_func(cell) do |_column, current_cell, current_model, current_iter|
      num = current_model.get_value(current_iter, WIDTH_COLUMN)
      current_cell.text = num.to_s
    end

    column = builder["treeviewcolumn2"]
    cell = builder["cellrenderertext2"]
    column.set_cell_data_func(cell) do |_column, current_cell, current_model, current_iter|
      num = current_model.get_value(current_iter, HEIGHT_COLUMN)
      current_cell.text = num.to_s
    end

    column = builder["treeviewcolumn3"]
    cell = builder["cellrenderertext3"]
    column.set_cell_data_func(cell) do |_column, current_cell, current_model, current_iter|
      num = current_model.get_value(current_iter, WIDTH_COLUMN)
      current_cell.text = num.to_s
    end

    column = builder["treeviewcolumn4"]
    cell = builder["cellrenderertext4"]
    column.set_cell_data_func(cell) do |_column, current_cell, current_model, current_iter|
      num = current_model.get_value(current_iter, HEIGHT_COLUMN)
      current_cell.text = num.to_s
    end

    column = builder["treeviewcolumn5"]
    cell = builder["cellrenderertext5"]
    column.set_cell_data_func(cell) do |_column, current_cell, current_model, current_iter|
      num = current_model.get_value(current_iter, AREA_COLUMN)
      current_cell.text = num.to_s
    end

    column = builder["treeviewcolumn6"]
    cell = builder["cellrendererpixbuf1"]
    column.add_attribute(cell, "visible", SQUARE_COLUMN)

    store = builder["liststore1"]
    tree = builder["treeview2"]

    types = [Integer, Integer, Integer, TrueClass]

    model = Gtk::TreeModelFilter.new(store)
    model.set_modify_func(*types) do |filter_model, filter_iter, filter_column|
      value = nil
      child_model = filter_model.model
      child_iter = filter_model.convert_iter_to_child_iter(filter_iter)
      width = child_model.get_value(child_iter, WIDTH_COLUMN)
      height = child_model.get_value(child_iter, HEIGHT_COLUMN)
      case filter_column
      when WIDTH_COLUMN
        value = width
      when HEIGHT_COLUMN
        value = height
      when AREA_COLUMN
        value = width * height
      when SQUARE_COLUMN
        value = (width == height)
      end
      value
    end

    tree.model = model

    column = builder["treeviewcolumn7"]
    cell = builder["cellrenderertext6"]
    column.set_cell_data_func(cell) do |_column, current_cell, current_model, current_iter|
      num = current_model.get_value(current_iter, WIDTH_COLUMN)
      current_cell.text = num.to_s
    end

    column = builder["treeviewcolumn8"]
    cell = builder["cellrenderertext7"]
    column.set_cell_data_func(cell) do |_column, current_cell, current_model, current_iter|
      num = current_model.get_value(current_iter, HEIGHT_COLUMN)
      current_cell.text = num.to_s
    end

    tree = builder["treeview3"]

    model = Gtk::TreeModelFilter.new(store)
    model.set_visible_func do |current_model, current_iter|
      current_model.get_value(current_iter, WIDTH_COLUMN) < 10
    end

    tree.model = model

    if !window.visible?
      window.show_all
    else
      window.destroy
    end
    window
  end
end
