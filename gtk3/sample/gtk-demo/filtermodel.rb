# Copyright (c) 2016 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
=  Tree View/Filter Model

 This example demonstrates how GtkTreeModelFilter can be used not
 just to show a subset of the rows, but also to compute columns
 that are not actually present in the underlying model.
=end
class FiltermodelDemo
  WIDTH_COLUMN = 0
  HEIGHT_COLUMN = 1
  AREA_COLUMN = 2
  SQUARE_COLUMN = 3

  def initialize(main_window)
    initialize_builder

    @window = @builder["window1"]
    @window.screen = main_window.screen

    set_treeview_column_data_from_int_to_string("treeviewcolumn1",
                                                "cellrenderertext1",
                                                WIDTH_COLUMN)
    set_treeview_column_data_from_int_to_string("treeviewcolumn2",
                                                "cellrenderertext2",
                                                HEIGHT_COLUMN)

    set_treeview_column_data_from_int_to_string("treeviewcolumn3",
                                                "cellrenderertext3",
                                                WIDTH_COLUMN)

    set_treeview_column_data_from_int_to_string("treeviewcolumn4",
                                                "cellrenderertext4",
                                                HEIGHT_COLUMN)

    set_treeview_column_data_from_int_to_string("treeviewcolumn5",
                                                "cellrenderertext5",
                                                AREA_COLUMN)

    column = @builder["treeviewcolumn6"]
    cell = @builder["cellrendererpixbuf1"]
    column.add_attribute(cell, "visible", SQUARE_COLUMN)

    @store = @builder["liststore1"]

    initialize_tree_model_filter_computed

    set_treeview_column_data_from_int_to_string("treeviewcolumn7",
                                                "cellrenderertext6",
                                                WIDTH_COLUMN)

    set_treeview_column_data_from_int_to_string("treeviewcolumn8",
                                                "cellrenderertext7",
                                                HEIGHT_COLUMN)

    initialize_tree_model_filter_selected
  end

  def run
    if !@window.visible?
      @window.show_all
    else
      @window.destroy
    end
    @window
  end

  private

  def initialize_builder
    @builder = Gtk::Builder.new(:resource => "/filtermodel/filtermodel.ui")
    @builder.connect_signals {}
  end

  def set_treeview_column_data_from_int_to_string(column_name, cell_name, col_index)
    column = @builder[column_name]
    cell = @builder[cell_name]
    column.set_cell_data_func(cell) do |_col, current_cell, _current_model, current_iter|
      num = current_iter[col_index]
      current_cell.text = num.to_s
    end
  end

  def initialize_tree_model_filter_computed
    tree = @builder["treeview2"]

    types = [Integer, Integer, Integer, TrueClass]

    model = @store.create_filter
    model.set_modify_func(*types) do |filter_model, filter_iter, filter_column|
      value = nil
      child_iter = filter_model.convert_iter_to_child_iter(filter_iter)
      width = child_iter[WIDTH_COLUMN]
      height = child_iter[HEIGHT_COLUMN]
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
  end

  def initialize_tree_model_filter_selected
    tree = @builder["treeview3"]

    model = @store.create_filter
    model.set_visible_func do |_current_model, current_iter|
      current_iter[WIDTH_COLUMN] < 10
    end

    tree.model = model
  end
end
