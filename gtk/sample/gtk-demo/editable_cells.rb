# $Id: editable_cells.rb,v 1.5 2003/09/07 11:41:00 mutoh Exp $
=begin
= Tree View/Editable Cells

This demo demonstrates the use of editable cells in a Gtk::TreeView. If
you're new to the Gtk::TreeView widgets and associates, look into
the Gtk::ListStore example first.
=end
require 'common'

module Demo
  class EditableCells < BasicWindow
    Item = Struct.new('Item', :number, :product, :editable)
    COLUMN_NUMBER, COLUMN_PRODUCT, COLUMN_EDITABLE, NUM_COLUMNS = *(0..4).to_a
    
    def initialize
      super('Shopping list')
      self.border_width = 5

      vbox = Gtk::VBox.new(false, 5)
      add(vbox)

      vbox.pack_start(Gtk::Label.new('Shopping list (you can edit the cells!)'),
		      false, false, 0)

      sw = Gtk::ScrolledWindow.new
      sw.shadow_type = Gtk::SHADOW_ETCHED_IN
      sw.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC)
      vbox.pack_start(sw, true, true, 0)

      # create model
      model = create_model 

      # create tree view
      treeview = Gtk::TreeView.new(model)
      treeview.rules_hint = true
      treeview.selection.mode = Gtk::SELECTION_SINGLE

      add_columns(treeview)

      sw.add(treeview)

      # some buttons
      hbox = Gtk::HBox.new(true, 4)
      vbox.pack_start(hbox, false, false, 0)

      button = Gtk::Button.new('Add item')
      button.signal_connect('clicked') do
	add_item(model)
      end
      hbox.pack_start(button, true, true, 0)

      button = Gtk::Button.new('Remove item')
      button.signal_connect('clicked') do
	remove_item(treeview)
      end
      hbox.pack_start(button, true, true, 0)

      set_default_size(320, 200)
    end

    def create_model
      # create array
      @articles = []

      add_items

      # create list store
      model = Gtk::ListStore.new(Integer, String, TrueClass)

      # add items
      @articles.each do |article|
	iter = model.append

	article.each_with_index do |value, index|
	  iter.set_value(index, value)
	end
      end
      return model
    end

    def add_items
      item = Item.new(3, 'bottles of coke', true)
      @articles.push(item)

      item = Item.new(5, 'packages of noodles', true)
      @articles.push(item)

      item = Item.new(2, 'packages of chocolate chip cookies', true)
      @articles.push(item)

      item = Item.new(1, 'can vanilla ice cream', true)
      @articles.push(item)

      item = Item.new(6, 'eggs', true)
      @articles.push(item)
    end

    def add_columns(treeview)
      model = treeview.model
      
      # number column
      renderer = Gtk::CellRendererText.new
      renderer.signal_connect('edited') do |*args|
	cell_edited(*args.push(model))
      end
      treeview.insert_column(-1, 'Number', renderer,
			     {
			       :text => COLUMN_NUMBER,
			       :editable => COLUMN_EDITABLE,
			     })
      def renderer.column
	COLUMN_NUMBER
      end

      # product column
      renderer = Gtk::CellRendererText.new
      renderer.signal_connect('edited') do |*args|
	cell_edited(*args.push(model))
      end
      def renderer.column
	COLUMN_PRODUCT
      end
      treeview.insert_column(-1, 'Product', renderer,
			     {
			       :text => COLUMN_PRODUCT,
			       :editable => COLUMN_EDITABLE,
			     })
    end
    
    def cell_edited(cell, path_string, new_text, model)
      path = Gtk::TreePath.new(path_string)

      column = cell.column

      iter = model.get_iter(path)
      case column
      when COLUMN_NUMBER
	i = iter.path.indices[0]
	@articles[i].number = new_text.to_i
	iter.set_value(column, @articles[i].number)
      when COLUMN_PRODUCT
	i = iter.path.indices[0]
	@articles[i].product = new_text
	iter.set_value(column, @articles[i].product)
      end
    end

    def add_item(model)
      foo = Item.new(0, 'Description here', true)
      @articles.concat([foo])

      iter = model.append
      foo.each_with_index do |value, index|
	iter.set_value(index, value)
      end
    end

    def remove_item(treeview)
      model = treeview.model
      selection = treeview.selection

      if iter = selection.selected
	@articles.delete_at(iter.path.indices[0])
	model.remove(iter)
      end
    end
  end
end
