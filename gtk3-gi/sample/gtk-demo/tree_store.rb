# Copyright (c) 2003-2005 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
# $Id: tree_store.rb,v 1.7 2005/02/06 18:25:13 kzys Exp $
=begin
= Tree View/Tree Store

The Gtk::TreeStore is used to store data in tree form, to be
used later on by a Gtk::TreeView to display it. This demo builds
a simple Gtk::TreeStore and displays it. If you're new to the
GtkTreeView widgets and associates, look into the Gtk::ListStore
example first.
=end
require 'common'

module Demo
  class TreeStore < BasicWindow
    # TreeItem structure
    TreeItem = Struct.new('TreeItem',
                          :label,
                          :alex, :havoc, :tim, :owen, :dave,
                          :world_holiday) # shared by the European hackers

    # columns
    (
     HOLIDAY_NAME_COLUMN,
     ALEX_COLUMN,
     HAVOC_COLUMN,
     TIM_COLUMN,
     OWEN_COLUMN,
     DAVE_COLUMN,

     VISIBLE_COLUMN,
     WORLD_COLUMN,
     NUM_COLUMNS,
     ) = *(0..9).to_a


    # tree data
    TOPLEVEL = [
      ['January',
        [
          ["New Years Day", true, true, true, true, false, true],
          ["Presidential Inauguration", false, true, false, true, false, false],
          ["Martin Luther King Jr. day", false, true, false, true, false, false],
        ]
      ],
      ['February',
        [
          ["Presidents' Day", false, true, false, true, false, false],
          ["Groundhog Day", false, false, false, false, false, false],
          ["Valentine's Day", false, false, false, false, true, true],
        ]
      ],
      ['March',
        [
          ["National Tree Planting Day", false, false, false, false, false, false],
          [ "St Patrick's Day", false, false, false, false, false, true],
        ]
      ],
      ['April',
        [
          [ "April Fools' Day", false, false, false, false, false, true ],
          [ "Army Day", false, false, false, false, false, false ],
          [ "Earth Day", false, false, false, false, false, true ],
          [ "Administrative Professionals' Day", false, false, false, false, false, false ],
        ]],
      ['May',
        [
          [ "Nurses' Day", false, false, false, false, false, false ],
          [ "National Day of Prayer", false, false, false, false, false, false ],
          [ "Mothers' Day", false, false, false, false, false, true ],
          [ "Armed Forces Day", false, false, false, false, false, false ],
          [ "Memorial Day", true, true, true, true, false, true ],

        ]
      ],
      ['June',
        [
          [ "June Fathers' Day", false, false, false, false, false, true ],
          [ "Juneteenth (Liberation of Slaves)", false, false, false, false, false, false ],
          [ "Flag Day", false, true, false, true, false, false ],
        ]
      ],
      ['July',
        [
          [ "Parents' Day", false, false, false, false, false, true ],
          [ "Independence Day", false, true, false, true, false, false ],
        ]
      ],
      ['August',
        [
          [ "Air Force Day", false, false, false, false, false, false ],
          [ "Coast Guard Day", false, false, false, false, false, false ],
          [ "Friendship Day", false, false, false, false, false, false ],
        ]
      ],
      ['September',
        [
          [ "Grandparents' Day", false, false, false, false, false, true ],
          [ "Citizenship Day or Constitution Day", false, false, false, false, false, false ],
          [ "Labor Day", true, true, true, true, false, true ],
        ]
      ],
      ['October',
        [
          [ "National Children's Day", false, false, false, false, false, false ],
          [ "Bosses' Day", false, false, false, false, false, false ],
          [ "Sweetest Day", false, false, false, false, false, false ],
          [ "Mother-in-Law's Day", false, false, false, false, false, false ],
          [ "Navy Day", false, false, false, false, false, false ],
          [ "Columbus Day", false, true, false, true, false, false ],
          [ "Halloween", false, false, false, false, false, true ],
        ]
      ],
      ['November',
        [
          [ "Marine Corps Day", false, false, false, false, false, false ],
          [ "Veterans' Day", true, true, true, true, false, true ],
          [ "Thanksgiving", false, true, false, true, false, false ],
        ]
      ],
      ['December',
        [
          [ 'Pearl Harbor Remembrance Day', false, false, false, false, false, false ],
          [ 'Christmas', true, true, true, true, false, true ],
          [ 'Kwanzaa', false, false, false, false, false, false ],
        ]
      ],
    ].collect do |month_name, holidays|
      [
        month_name,
        holidays.collect do |args| TreeItem.new(args) end,
      ]
    end

    def initialize
      # create window, etc
      super('Card planning sheet')

      vbox = Gtk::VBox.new(false, 8)
      vbox.border_width = 8
      add(vbox)

      vbox.pack_start(Gtk::Label.new("Jonathan's Holiday Card Planning Sheet"),
                      :expand => false, :fill => false, :padding => 0)

      sw = Gtk::ScrolledWindow.new(nil, nil)
      sw.shadow_type = Gtk::SHADOW_ETCHED_IN
      sw.set_policy(:automatic,
                    :automatic)
      vbox.pack_start(sw, :expand => true, :fill => true, :padding => 0)

      # create model
      model = create_model

      # create tree view
      treeview = Gtk::TreeView.new(model)
      treeview.rules_hint = true
      treeview.selection.mode = Gtk::SELECTION_MULTIPLE

      add_columns(treeview)

      sw.add(treeview)

      # expand all rows after the treeview widget has been realized
      treeview.signal_connect('realize') do
        treeview.expand_all
      end
      set_default_size(650, 400)
    end

    def create_model
      # create tree store
      model = Gtk::TreeStore.new(String,
                                 TrueClass,
                                 TrueClass,
                                 TrueClass,
                                 TrueClass,
                                 TrueClass,
                                 TrueClass,
                                 TrueClass)

      # add data to the tree store
      TOPLEVEL.each do |month_name, holidays|
        iter = model.append(nil)

        iter[HOLIDAY_NAME_COLUMN] = month_name
        (ALEX_COLUMN..DAVE_COLUMN).each do |index|
          iter[index] = false
        end

        # add children
        holidays.each do |holiday|
          child_iter = model.append(iter)
          child_iter[HOLIDAY_NAME_COLUMN] = holiday.label[0]
          %w(alex havoc tim owen dave).each_with_index do |person, i|
            child_iter[ALEX_COLUMN + i] = holiday[person]
          end
          child_iter[VISIBLE_COLUMN] = true
          child_iter[WORLD_COLUMN] = holiday.world_holiday
        end

      end

      return model
    end

    def add_columns(treeview)
      model = treeview.model

      # column for holiday names
      renderer = Gtk::CellRendererText.new
      renderer.xalign = 0.0

      col_offset = treeview.insert_column(-1, 'Holiday',
                                          renderer,
                                          'text' => HOLIDAY_NAME_COLUMN)
      column = treeview.get_column(col_offset - 1)
      column.clickable = true

      [
        ['Alex',  true ],
        ['Havoc', false],
        ['Tim',   true ],
        ['Owen',  false],
        ['Dave',  false],
      ].each_with_index do |args, i|
        person, european = *args
        renderer = Gtk::CellRendererToggle.new
        renderer.xalign = 0.0
        eval(%Q[def renderer.column; #{ALEX_COLUMN + i}; end])

        renderer.signal_connect('toggled') do |cell, path|
          item_toggled(cell, path, model)
        end

        attributes =  {
          'active' => (ALEX_COLUMN + i),
          'visible' => VISIBLE_COLUMN,
        }
        if european
          attributes['activatable'] = WORLD_COLUMN
        end
        col_offset = treeview.insert_column(-1, person, renderer, attributes)

        column = treeview.get_column(col_offset - 1)
        column.sizing = Gtk::TreeViewColumn::FIXED
        column.fixed_width = 50
        column.clickable = true
      end
    end

    def item_toggled (cell, path_str, model)
      path = Gtk::TreePath.new(path_str)

      column = cell.column

      # get toggled iter
      iter = model.get_iter(path)
      toggle_item = iter[column]

      # do something with the value
      toggle_item ^= 1

      # set new value
      iter[column] = toggle_item
    end
  end
end
