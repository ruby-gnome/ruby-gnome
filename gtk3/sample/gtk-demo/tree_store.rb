# Copyright (c) 2017 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
=  Tree View/Tree Store

 The GtkTreeStore is used to store data in tree form, to be
 used later on by a GtkTreeView to display it. This demo builds
 a simple GtkTreeStore and displays it. If you're new to the
 GtkTreeView widgets and associates, look into the GtkListStore
 example first.
=end
class TreeStoreDemo
  TreeItem = Struct.new("TreeItem",
                        :label, :alex, :havoc, :tim, :owen, :dave,
                        :world_holiday, :children)
  HOLIDAY_NAME_COLUMN, ALEX_COLUMN, HAVOC_COLUMN, TIM_COLUMN, OWEN_COLUMN,
  DAVE_COLUMN, VISIBLE_COLUMN, WORLD_COLUMN, NUM_COLUMNS = (0..8).to_a

  January = [
    ["New Years Day", true, true, true, true, false, true, nil],
    ["Presidential Inauguration", false, true, false, true, false, false, nil],
    ["Martin Luther King Jr. day", false, true, false, true, false, false, nil]
  ].collect do |ary|
    TreeItem.new(*ary)
  end

  February = [
    ["Presidents' Day", false, true, false, true, false, false, nil],
    ["Groundhog Day", false, false, false, false, false, false, nil],
    ["Valentine's Day", false, false, false, false, true, true, nil]
  ].collect do |ary|
    TreeItem.new(*ary)
  end

  March = [
    ["National Tree Planting Day", false, false, false, false, false, false, nil],
    ["St Patrick's Day", false, false, false, false, false, true, nil]
  ].collect do |ary|
    TreeItem.new(*ary)
  end

  April = [
    ["April Fools' Day", false, false, false, false, false, true, nil],
    ["Army Day", false, false, false, false, false, false, nil],
    ["Earth Day", false, false, false, false, false, true, nil],
    ["Administrative Professionals' Day", false, false, false, false, false, false, nil]
  ].collect do |ary|
    TreeItem.new(*ary)
  end

  May = [
    ["Nurses' Day", false, false, false, false, false, false, nil],
    ["National Day of Prayer", false, false, false, false, false, false, nil],
    ["Mothers' Day", false, false, false, false, false, true, nil],
    ["Armed Forces Day", false, false, false, false, false, false, nil],
    ["Memorial Day", true, true, true, true, false, true, nil]
  ].collect do |ary|
    TreeItem.new(*ary)
  end

  June = [
    ["June Fathers' Day", false, false, false, false, false, true, nil],
    ["Juneteenth (Liberation of Slaves)", false, false, false, false, false, false, nil],
    ["Flag Day", false, true, false, true, false, false, nil]
  ].collect do |ary|
    TreeItem.new(*ary)
  end

  July = [
    ["Parents' Day", false, false, false, false, false, true, nil],
    ["Independence Day", false, true, false, true, false, false, nil]
  ].collect do |ary|
    TreeItem.new(*ary)
  end

  August = [
    ["Air Force Day", false, false, false, false, false, false, nil],
    ["Coast Guard Day", false, false, false, false, false, false, nil],
    ["Friendship Day", false, false, false, false, false, false, nil]
  ].collect do |ary|
    TreeItem.new(*ary)
  end

  September = [
    ["Grandparents' Day", false, false, false, false, false, true, nil],
    ["Citizenship Day or Constitution Day", false, false, false, false, false, false, nil],
    ["Labor Day", true, true, true, true, false, true, nil]
  ].collect do |ary|
    TreeItem.new(*ary)
  end

  October = [
    ["National Children's Day", false, false, false, false, false, false, nil],
    ["Bosses' Day", false, false, false, false, false, false, nil],
    ["Sweetest Day", false, false, false, false, false, false, nil],
    ["Mother-in-Law's Day", false, false, false, false, false, false, nil],
    ["Navy Day", false, false, false, false, false, false, nil],
    ["Columbus Day", false, true, false, true, false, false, nil],
    ["Halloween", false, false, false, false, false, true, nil]
  ].collect do |ary|
    TreeItem.new(*ary)
  end

  November = [
    ["Marine Corps Day", false, false, false, false, false, false, nil],
    ["Veterans' Day", true, true, true, true, false, true, nil],
    ["Thanksgiving", false, true, false, true, false, false, nil]
  ].collect do |ary|
    TreeItem.new(*ary)
  end

  December = [
    ["Pearl Harbor Remembrance Day", false, false, false, false, false, false, nil],
    ["Christmas", true, true, true, true, false, true, nil],
    ["Kwanzaa", false, false, false, false, false, false, nil]
  ].collect do |ary|
    TreeItem.new(*ary)
  end

  Toplevel = [
    ["January", false, false, false, false, false, false, January],
    ["February", false, false, false, false, false, false, February],
    ["March", false, false, false, false, false, false, March],
    ["April", false, false, false, false, false, false, April],
    ["May", false, false, false, false, false, false, May],
    ["June", false, false, false, false, false, false, June],
    ["July", false, false, false, false, false, false, July],
    ["August", false, false, false, false, false, false, August],
    ["September", false, false, false, false, false, false, September],
    ["October", false, false, false, false, false, false, October],
    ["November", false, false, false, false, false, false, November],
    ["December", false, false, false, false, false, false, December]
  ].collect do |ary|
    TreeItem.new(*ary)
  end

  def initialize(main_window)
    @window = Gtk::Window.new(:toplevel)
    @window.screen = main_window.screen
    @window.title = "Tree Store"

    vbox = Gtk::Box.new(:vertical, 8)
    vbox.margin = 8
    @window.add(vbox)
    label = Gtk::Label.new("Jonathan's Holiday Card Planning Sheet")
    vbox.pack_start(label)

    sw = Gtk::ScrolledWindow.new
    sw.shadow_type = :etched_in
    sw.set_policy(:automatic, :automatic)
    vbox.pack_start(sw, :expand => true, :fill => true, :padding => 0)

    create_model
    @treeview = Gtk::TreeView.new(@model)
    @treeview.selection.mode = :multiple

    add_columns
    sw.add(@treeview)

    # Expand all rows after the treeview widget has been realized
    @treeview.signal_connect("realize", &:expand_all)
    @window.set_default_size(650, 400)
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

  def create_model
    @model = Gtk::TreeStore.new(String, TrueClass, TrueClass, TrueClass,
                                TrueClass, TrueClass, TrueClass, TrueClass)
    Toplevel.each do |month|
      iter = @model.append(nil)
      iter[HOLIDAY_NAME_COLUMN] = month.label
      iter[ALEX_COLUMN] = false
      iter[HAVOC_COLUMN] = false
      iter[TIM_COLUMN] = false
      iter[OWEN_COLUMN] = false
      iter[DAVE_COLUMN] = false
      iter[VISIBLE_COLUMN] = false
      iter[WORLD_COLUMN] = false

      holidays = month.children
      # add children
      holidays.each do |holiday|
        child_iter = @model.append(iter)
        child_iter[HOLIDAY_NAME_COLUMN] = holiday.label
        child_iter[ALEX_COLUMN] = holiday.alex
        child_iter[HAVOC_COLUMN] = holiday.havoc
        child_iter[TIM_COLUMN] = holiday.tim
        child_iter[OWEN_COLUMN] = holiday.owen
        child_iter[DAVE_COLUMN] = holiday.dave
        child_iter[VISIBLE_COLUMN] = true
        child_iter[WORLD_COLUMN] = holiday.world_holiday
      end
    end
  end

  def add_toggle_column(label, col_index, attributes)
    renderer = Gtk::CellRendererToggle.new
    renderer.xalign = 0.0
    renderer.signal_connect "toggled" do |_cell, path_str|
      path = Gtk::TreePath.new(path_str)
      iter = @model.get_iter(path)
      toggle_item = iter[col_index]

      toggle_item = !toggle_item
      iter[col_index] = toggle_item
    end

    col_offset = @treeview.insert_column(-1, label, renderer, attributes)
    column = @treeview.get_column(col_offset - 1)
    column.sizing = :fixed
    column.clickable = true
  end

  def add_columns
    # column for holiday names
    renderer = Gtk::CellRendererText.new
    renderer.xalign = 0.0

    col_offset = @treeview.insert_column(-1, "Holiday", renderer,
                                         :text => HOLIDAY_NAME_COLUMN)
    column = @treeview.get_column(col_offset - 1)
    column.clickable = true

    add_toggle_column("Alex", ALEX_COLUMN,
                      :active => ALEX_COLUMN,
                      :visible => VISIBLE_COLUMN,
                      :activatable => WORLD_COLUMN)

    add_toggle_column("Havoc", HAVOC_COLUMN,
                      :active => HAVOC_COLUMN,
                      :visible => VISIBLE_COLUMN)

    add_toggle_column("Tim", TIM_COLUMN,
                      :active => TIM_COLUMN,
                      :visible => VISIBLE_COLUMN,
                      :activatable => WORLD_COLUMN)

    add_toggle_column("Owen", OWEN_COLUMN,
                      :active => OWEN_COLUMN,
                      :visible => VISIBLE_COLUMN)

    add_toggle_column("Dave", DAVE_COLUMN,
                      :active => DAVE_COLUMN,
                      :visible => VISIBLE_COLUMN)
  end
end
