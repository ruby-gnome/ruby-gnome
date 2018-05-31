# Copyright (c) 2016 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
=  Tree View/Editable Cells

 This demo demonstrates the use of editable cells in a GtkTreeView. If
 you're new to the GtkTreeView widgets and associates, look into
 the GtkListStore example first. It also shows how to use the
 GtkCellRenderer::editing-started signal to do custom setup of the
 editable widget.

 The cell renderers used in this demo are GtkCellRendererText,
 GtkCellRendererCombo and GtkCellRendererProgress.
=end
class EditableCellsDemo
  Item = Struct.new("Item", :number, :product, :yummy)
  COL_ITEM_NUM, COL_ITEM_PROD, COL_ITEM_YUMMY = 0, 1, 2
  COL_NUM_TEXT = 0

  def initialize(main_window)
    @window = Gtk::Window.new(:toplevel)
    @window.screen = main_window.screen
    @window.title = "Editable Cells"

    vbox = Gtk::Box.new(:vertical, 5)
    vbox.margin = 5
    @window.add(vbox)
    label = Gtk::Label.new("Shopping list (you can edit the cells)")
    vbox.pack_start(label, :expand => false, :fill => true, :padding => 0)

    sw = Gtk::ScrolledWindow.new
    sw.shadow_type = :etched_in
    sw.set_policy(:automatic, :automatic)
    vbox.pack_start(sw, :expand => true, :fill => true, :padding => 0)

    # create models
    create_items_model
    create_numbers_model

    # create tree view
    @treeview = Gtk::TreeView.new(@items_model)
    @treeview.selection.mode = :single

    add_columns

    sw.add(@treeview)

    # Some buttons
    hbox = Gtk::Box.new(:horizontal, 4)
    hbox.homogeneous = true
    vbox.pack_start(hbox, :expand => false, :fill => false, :padding => 0)

    button = Gtk::Button.new(:label => "Add item")
    button.signal_connect("clicked") { add_item }
    hbox.pack_start(button, :expand => true, :fill => true, :padding => 0)

    button = Gtk::Button.new(:label => "Remove item")
    button.signal_connect("clicked") { remove_item }

    hbox.pack_start(button, :expand => true, :fill => true, :padding => 0)
    @window.set_default_size(320, 200)
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

  def add_item
    return if @items.nil?
    item = Item.new(0, "Description here", 50)
    @items << item

    # Insert a row below the current one
    path = @treeview.cursor[0]
    iter = if path
             current = @items_model.get_iter(path)
             @items_model.insert_after(current)
           else
             @items_model.insert(-1)
           end

    # set the data for the new row
    item.each_with_index do |value, index|
      iter[index] = value
    end

    # Move focus to the new row
    path = @items_model.get_path(iter)
    column = @treeview.get_column(0)
    @treeview.set_cursor(path, column, false)
  end

  def remove_item
    selection = @treeview.selection
    iter = selection.selected
    if iter
      path = @items_model.get_path(iter)
      i = path.indices[0]
      @items_model.remove(iter)
      @items.delete_at(i)
    end
  end

  def add_items
    @items << Item.new(3, "bottles of coke", 20)
    @items << Item.new(5, "packages of noodles", 50)
    @items << Item.new(2, "packages of chocolate chip cookies", 90)
    @items << Item.new(1, "can vanilla ice cream", 60)
    @items << Item.new(6, "eggs", 10)
  end

  def create_items_model
    @items = []
    add_items

    # Create list store
    @items_model = Gtk::ListStore.new(Integer, String, Integer, TrueClass)
    @items.each do |item|
      iter = @items_model.append
      iter[COL_ITEM_NUM] = item.number
      iter[COL_ITEM_PROD] = item.product
      iter[COL_ITEM_YUMMY] = item.yummy
    end
  end

  def create_numbers_model
    @numbers_model = Gtk::ListStore.new(String, Integer)
    (1..10).each do |n|
      iter = @numbers_model.append
      iter[COL_NUM_TEXT] = n.to_s
    end
  end

  def add_columns
    add_number_column
    add_product_column
    add_yummy_column
  end

  def add_number_column
    renderer = Gtk::CellRendererCombo.new
    renderer.model = @numbers_model
    renderer.text_column = COL_NUM_TEXT
    renderer.has_entry = false
    renderer.editable = true

    renderer.signal_connect "edited" do |cell, path, new_text|
      cell_edited(cell, path, new_text)
    end

    renderer.signal_connect "editing-started" do |_cell, editable, path|
      editable.set_row_separator_func do |model, iter|
        path = model.get_path(iter)
        idx = path.indices[0]
        idx == 5
      end
    end

    def renderer.column
      COL_ITEM_NUM
    end

    @treeview.insert_column(-1, "Number", renderer, :text => COL_ITEM_NUM)
  end

  def add_product_column
    renderer = Gtk::CellRendererText.new
    renderer.editable = true
    renderer.signal_connect "edited" do |cell, path, new_text|
      cell_edited(cell, path, new_text)
    end

    def renderer.column
      COL_ITEM_PROD
    end

    @treeview.insert_column(-1, "Product", renderer, :text => COL_ITEM_PROD)
  end

  def add_yummy_column
    renderer = Gtk::CellRendererProgress.new
    def renderer.column
      COL_ITEM_YUMMY
    end
    @treeview.insert_column(-1, "Yummy", renderer, :value => COL_ITEM_YUMMY)
  end

  def cell_edited(cell, path_string, new_text)
    path = Gtk::TreePath.new(path_string)
    column = cell.column
    iter = @items_model.get_iter(path)

    case column
    when COL_ITEM_NUM
      i = path.indices[0]
      @items[i].number = new_text.to_i
      iter.set_value(column, @items[i].number)
    when COL_ITEM_PROD
      i = path.indices[0]
      @items[i].product = new_text
      iter.set_value(column, @items[i].product)
    end
  end
end
