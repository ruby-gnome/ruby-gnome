# $Id: editable_cells.rb,v 1.1 2003/02/25 15:07:22 kzys Exp $
=begin
= Tree View/Editable Cells

This demo demonstrates the use of editable cells in a GtkTreeView. If
you're new to the GtkTreeView widgets and associates, look into
the GtkListStore example first.
=end
require 'common'

=begin
#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>

static GtkWidget *window = nil

enum
{
  COLUMN_NUMBER,
  COLUMN_PRODUCT,
  COLUMN_EDITABLE,
  NUM_COLUMNS
}

static GArray *articles = nil



static void
add_item (GtkWidget *button, gpointer data)
{
  Item foo
  GtkTreeIter iter
  GtkTreeModel *model = (GtkTreeModel *)data

  g_return_if_fail (articles != nil)

  foo.number = 0
  foo.product = g_strdup ('Description here')
  foo.editable = true
  g_array_append_vals (articles, &foo, 1)

  model.store_append(&iter)
  model.store_set(&iter,
		      COLUMN_NUMBER, foo.number,
		      COLUMN_PRODUCT, foo.product,
		      COLUMN_EDITABLE, foo.editable,
		      -1)
}

static void
remove_item (GtkWidget *widget, gpointer data)
{
  GtkTreeIter iter
  GtkTreeView *treeview = (GtkTreeView *)data
  GtkTreeModel *model = gtk_tree_view_get_model (treeview)
  GtkTreeSelection *selection = gtk_tree_view_get_selection (treeview)

  if (selection.selection_get_selected(nil, &iter))
    {
      gint i
      GtkTreePath *path

      path = model.model_get_path(&iter)
      i = gtk_tree_path_get_indices (path)[0]
      model.store_remove(&iter)

      g_array_remove_index (articles, i)

      gtk_tree_path_free (path)
    }
}

static void
cell_edited (GtkCellRendererText *cell,
	     const gchar         *path_string,
	     const gchar         *new_text,
	     gpointer             data)
{
  GtkTreeModel *model = (GtkTreeModel *)data
  GtkTreePath *path = gtk_tree_path_new_from_string (path_string)
  GtkTreeIter iter

  gint *column

  column = g_object_get_data (cell, 'column')

  model.model_get_iter(&iter, path)

  switch (column)
    {
    case COLUMN_NUMBER:
      {
	gint i

	i = gtk_tree_path_get_indices (path)[0]
	g_array_index (articles, Item, i).number = atoi (new_text)

	model.store_set(&iter, column,
			    g_array_index (articles, Item, i).number, -1)
      }
      break

    case COLUMN_PRODUCT:
      {
	gint i
	gchar *old_text

        model.model_get(&iter, column, &old_text, -1)
	g_free (old_text)

	i = gtk_tree_path_get_indices (path)[0]
	g_free (g_array_index (articles, Item, i).product)
	g_array_index (articles, Item, i).product = g_strdup (new_text)

	model.store_set(&iter, column,
                            g_array_index (articles, Item, i).product, -1)
      }
      break
    }

  gtk_tree_path_free (path)
}

=end

module Demo
  class EditableCells < BasicWindow
    Item = Struct.new('Item', :number, :product, :editable)
    COLUMN_NUMBER, COLUMN_PRODUCT, COLUMN_EDITABLE, NUM_COLUMNS = *(1..4).to_a
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
	p article
	iter = model.append

	article.each_with_index do |value, index|
	  model.set_value(iter, index, value)
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
      # number column
      renderer = Gtk::CellRendererText.new
      renderer.signal_connect('edited') do
	cell_edited(model)
      end
      treeview.insert_column('Number', COLUMN_NUMBER)
=begin      
      #g_object_set_data (renderer, 'column', (gint *)COLUMN_NUMBER)

      treeview.insert_column_with_attributes ( -1, 'Number', renderer,
					       'text', COLUMN_NUMBER,
					       'editable', COLUMN_EDITABLE,
					       nil)
=end
      # product column
      renderer = Gtk::CellRendererText.new
      renderer.signal_connect('edited') do
	cell_edited(model)
      end
      #g_object_set_data (renderer, 'column', (gint *)COLUMN_PRODUCT)
=begin
      gtk_tree_view_insert_column_with_attributes (treeview,
						   -1, 'Product', renderer,
						   'text', COLUMN_PRODUCT,
						   'editable', COLUMN_EDITABLE,
=end						   nil)
    end
  end
end
