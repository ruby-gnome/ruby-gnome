#!/usr/bin/env ruby
require 'libglade2'

class Treeview
  def initialize(path)
    @glade = GladeXML.new(path) {|handler| method(handler)}
    @treeview = @glade.get_widget("treeview1")
    @model = Gtk::TreeStore.new(String)
    column = Gtk::TreeViewColumn.new("Test", Gtk::CellRendererText.new, {:text => 0})
    @treeview.append_column(column)
    root_iter = @model.append(nil)
    root_iter.set_value(0, "Root")
    child1 = @model.append(root_iter)
    child1.set_value(0, "Child1")
    child2 = @model.append(root_iter)
    child2.set_value(0, "Child2")
    child3 = @model.append(child1)
    child3.set_value(0, "Child3")
    @treeview.set_model(@model)
    @treeview.set_size_request(200, 200)
  end
end

Gtk.init
Treeview.new("treeview.glade")
Gtk.main
