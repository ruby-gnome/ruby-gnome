# Standalone demo of ColumnView in gtk4.

require "gtk4"

class DataObject < GLib::Object
    attr_accessor :fruit, :color
    def initialize(fruit, color)
      super()
      @fruit = fruit
      @color = color
    end
end

def setup(factory, cell)
    # Setup the widget to show in the Gtk.Listview
    label = Gtk::Label.new()
    label.xalign = 0.0
    cell.set_child(label)
end

def bind_fruit(factory, cell)
    # bind data from the store object to the widget
    label = cell.child # get_child()
    obj = cell.item # get_item()
    label.set_label(obj.fruit)
end

def bind_color(factory, cell)
    # bind data from the store object to the widget"""
    label = cell.child
    obj = cell.item
    label.label = obj.color
end

def on_activate(app)
    win = Gtk::ApplicationWindow.new(app)
    win.set_default_size(400, 500)
    win.modal = true  
    sw = Gtk::ScrolledWindow.new()
    column_view = Gtk::ColumnView.new()
    column_view.show_row_separators = true
    column_view.show_column_separators = true
    fruit_col = Gtk::ColumnViewColumn.new("Fruit")
    fruit_col.set_resizable(true)
    fruit_col.set_fixed_width(200)
    fruit_factory = Gtk::SignalListItemFactory.new()
    fruit_factory.signal_connect("setup") { |factory, cell| setup(factory, cell) }
    fruit_factory.signal_connect("bind") { |factory, cell|  bind_fruit(factory, cell) } # item.get_child(), item) }
    fruit_col.set_factory(fruit_factory)
    column_view.append_column(fruit_col)
    color_col = Gtk::ColumnViewColumn.new("Color")
    color_col.set_resizable(true)
    color_col.set_fixed_width(200)
    color_factory = Gtk::SignalListItemFactory.new()
    color_factory.signal_connect("setup") { |factory, cell| setup(factory, cell) }
    color_factory.signal_connect("bind") { |factory, cell|  bind_color(cell.child, cell) }
    color_col.set_factory(color_factory)
    column_view.append_column(color_col)
    selection = Gtk::SingleSelection.new()
    store = Gio::ListStore.new(DataObject)
    selection.set_model(store)
    column_view.set_model(selection)
    store.append(DataObject.new("Apple", "Green"))
    store.append(DataObject.new("Apple", "Yellow"))
    store.append(DataObject.new("Apple", "Red"))
    store.append(DataObject.new("Banana", "Yellow"))
    sw.set_child(column_view)
    win.set_child(sw)
    win.present()
end

app = Gtk::Application.new("org.gtk.ColumnView_Example")
app.signal_connect("activate") do |a|
  on_activate(a)
end
app.run(nil)
