# Copyright (c) 2017 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
=  Combo Boxes

 The GtkComboBox widget allows to select one option out of a list.
 The GtkComboBoxEntry additionally allows the user to enter a value
 that is not in the list of options.

 How the options are displayed is controlled by cell renderers.
=end

Icon = Struct.new(:name, :label)
CapitalItem = Struct.new(:group, :name)

class ComboboxDemo
  ICON_NAME_COL = 0
  TEXT_COL = 1

  def initialize(main_window)
    @window = Gtk::Window.new(:toplevel)
    @window.screen = main_window.screen
    @window.title = "Combo Boxes"
    vbox = Gtk::Box.new(:vertical, 2)
    vbox.margin = 10
    @window.add(vbox)

    # A combobox demonstrating cell renderers, separators and
    # insensitive rows.

    frame = Gtk::Frame.new("Items with icons")
    vbox.pack_start(frame, :expand => false, :fill => false, :padding => 0)

    box = Gtk::Box.new(:vertical, 0)
    box.margin = 5
    frame.add(box)

    store = create_icon_store
    combo = Gtk::ComboBox.new(:model => store)
    box.add(combo)

    renderer = Gtk::CellRendererPixbuf.new
    combo.pack_start(renderer, false)
    combo.set_attributes(renderer, "icon-name" => ICON_NAME_COL)

    combo.set_cell_data_func(renderer) do |_layout, cell_renderer, model, iter|
      set_sensitive(cell_renderer, model, iter)
    end

    renderer = Gtk::CellRendererText.new
    combo.pack_start(renderer, true)
    combo.set_attributes(renderer, "text" => TEXT_COL)

    combo.set_cell_data_func(renderer) do |_layout, cell_renderer, model, iter|
      set_sensitive(cell_renderer, model, iter)
    end

    combo.set_row_separator_func do |model, iter|
      separator?(model, iter)
    end

    combo.active = 0

    # A combobox demonstrating trees
    frame = Gtk::Frame.new("Where are we?")
    vbox.pack_start(frame, :expand => false, :fill => false, :padding => 0)

    box = Gtk::Box.new(:vertical, 0)
    box.margin = 5
    frame.add(box)

    store = create_capital_store
    combo = Gtk::ComboBox.new(:model => store)
    box.add(combo)

    renderer = Gtk::CellRendererText.new
    combo.pack_start(renderer, true)
    combo.set_attributes(renderer, "text" => 0)
    combo.set_cell_data_func(renderer) do |_layout, cell_renderer, model, iter|
      capital_sensitive?(cell_renderer, model, iter)
    end

    path = Gtk::TreePath.new([0, 8, -1])
    iter = store.get_iter(path)
    combo.set_active_iter(iter)

    # A GtkComboBoxEntry with validation
    frame = Gtk::Frame.new("Editable")
    vbox.pack_start(frame, :expand => true, :fill => true, :padding => 0)
    box = Gtk::Box.new(:vertical, 0)
    box.margin = 5
    frame.add(box)

    combo = Gtk::ComboBoxText.new(:entry => true)
    fill_combo_entry(combo)
    box.add(combo)

    entry = MaskEntry.new
    entry.mask = "^([0-9]*|One|Two|2\302\275|Three)$"

    combo.remove_child(combo.child)
    combo.add_child(entry)

    # A combobox with string IDs
    frame = Gtk::Frame.new("String IDs")
    vbox.pack_start(frame, :expand => false, :fill => false, :padding => 0)

    box = Gtk::Box.new(:vertical, 0)
    box.margin = 5
    frame.add(box)

    combo = Gtk::ComboBoxText.new
    combo.append("never", "Not visible")
    combo.append("when-active", "Visible")
    combo.append("always", "Always visible")
    box.add(combo)

    entry = Gtk::Entry.new
    combo.bind_property("active-id", entry, "text", :bidirectional)
    box.add(entry)
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

  def create_icon_store
    icons = []
    icons << Icon.new("dialog-warning", "Warning")
    icons << Icon.new("process-stop", "Stop")
    icons << Icon.new("document-new", "New")
    icons << Icon.new("edit-clear", "Clear")
    icons << Icon.new(nil, nil)
    icons << Icon.new("document-open", "Open")
    model = Gtk::ListStore.new(String, String)

    icons.each do |icon|
      iter = model.append
      if icon.name
        iter[ICON_NAME_COL] = icon.name
        iter[TEXT_COL] = icon.label
      else
        iter[ICON_NAME_COL] = nil
        iter[TEXT_COL] = "separator"
      end
    end
    model
  end

  # A GtkCellLayoutDataFunc that demonstrates how one can control
  # sensitivity of rows. This particular function does nothing
  # useful and just makes the second row insensitive.

  def set_sensitive(cell_renderer, model, iter)
    path = model.get_path(iter)
    indices = path.indices
    cell_renderer.sensitive = indices[0] != 1
  end

  def separator?(model, iter)
    path = model.get_path(iter)
    path.indices[0] == 4
  end

  def create_capital_store
    capitals = []
    capitals << CapitalItem.new("A - B", nil)
    capitals << CapitalItem.new(nil, "Albany")
    capitals << CapitalItem.new(nil, "Annapolis")
    capitals << CapitalItem.new(nil, "Atlanta")
    capitals << CapitalItem.new(nil, "Augusta")
    capitals << CapitalItem.new(nil, "Austin")
    capitals << CapitalItem.new(nil, "Baton Rouge")
    capitals << CapitalItem.new(nil, "Bismarck")
    capitals << CapitalItem.new(nil, "Boise")
    capitals << CapitalItem.new(nil, "Boston")
    capitals << CapitalItem.new("C - D", nil)
    capitals << CapitalItem.new(nil, "Carson City")
    capitals << CapitalItem.new(nil, "Charleston")
    capitals << CapitalItem.new(nil, "Cheyenne")
    capitals << CapitalItem.new(nil, "Columbia")
    capitals << CapitalItem.new(nil, "Columbus")
    capitals << CapitalItem.new(nil, "Concord")
    capitals << CapitalItem.new(nil, "Denver")
    capitals << CapitalItem.new(nil, "Des Moines")
    capitals << CapitalItem.new(nil, "Dover")
    capitals << CapitalItem.new("E - J", nil)
    capitals << CapitalItem.new(nil, "Frankfort")
    capitals << CapitalItem.new(nil, "Harrisburg")
    capitals << CapitalItem.new(nil, "Hartford")
    capitals << CapitalItem.new(nil, "Helena")
    capitals << CapitalItem.new(nil, "Honolulu")
    capitals << CapitalItem.new(nil, "Indianapolis")
    capitals << CapitalItem.new(nil, "Jackson")
    capitals << CapitalItem.new(nil, "Jefferson City")
    capitals << CapitalItem.new(nil, "Juneau")
    capitals << CapitalItem.new("K - O", nil)
    capitals << CapitalItem.new(nil, "Lansing")
    capitals << CapitalItem.new(nil, "Lincoln")
    capitals << CapitalItem.new(nil, "Little Rock")
    capitals << CapitalItem.new(nil, "Madison")
    capitals << CapitalItem.new(nil, "Montgomery")
    capitals << CapitalItem.new(nil, "Montpelier")
    capitals << CapitalItem.new(nil, "Nashville")
    capitals << CapitalItem.new(nil, "Oklahoma City")
    capitals << CapitalItem.new(nil, "Olympia")
    capitals << CapitalItem.new(nil, "P - S")
    capitals << CapitalItem.new(nil, "Phoenix")
    capitals << CapitalItem.new(nil, "Pierre")
    capitals << CapitalItem.new(nil, "Providence")
    capitals << CapitalItem.new(nil, "Raleigh")
    capitals << CapitalItem.new(nil, "Richmond")
    capitals << CapitalItem.new(nil, "Sacramento")
    capitals << CapitalItem.new(nil, "Salem")
    capitals << CapitalItem.new(nil, "Salt Lake City")
    capitals << CapitalItem.new(nil, "Santa Fe")
    capitals << CapitalItem.new(nil, "Springfield")
    capitals << CapitalItem.new(nil, "St. Paul")
    capitals << CapitalItem.new("T - Z", nil)
    capitals << CapitalItem.new(nil, "Tallahassee")
    capitals << CapitalItem.new(nil, "Topeka")
    capitals << CapitalItem.new(nil, "Trenton")
    model = Gtk::TreeStore.new(String)
    iter = nil
    iter2 = nil
    capitals.each do |capital|
      if capital.group
        iter = model.append(nil)
        iter[0] = capital.group
      else
        iter2 = model.append(iter)
        iter2[0] = capital.name
      end
    end
    model
  end

  def capital_sensitive?(cell, tree_model, iter)
    sensitive = !tree_model.iter_has_child(iter)
    cell.sensitive = sensitive
  end

  def fill_combo_entry(combo)
    combo.append_text("One")
    combo.append_text("Two")
    combo.append_text("2\302\275")
    combo.append_text("Three")
  end
end

class MaskEntry < Gtk::Entry
  type_register
  attr_writer :mask
  def initialize
    super
    signal_connect "changed" do
      set_background
    end
  end

  private

  def set_background
    return unless @mask
    if !GLib::Regex.match?(@mask, text)
      attrs = Pango::AttrList.new
      attrs.insert(Pango::AttrForeground.new(65_535, 32_767, 32_767))
      set_attributes(attrs)
    else
      set_attributes(nil)
    end
  end
end
