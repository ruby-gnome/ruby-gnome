# Copyright (c) 2016 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
=  Tree View/List Store

 The GtkListStore is used to store data in list form, to be used
 later on by a GtkTreeView to display it. This demo builds a
 simple GtkListStore and displays it.

=end
class ListStoreDemo
  Bug = Struct.new("Bug", :fixed, :number, :severity, :description)
  COLUMN_FIXED, COLUMN_NUMBER, COLUMN_SEVERITY, COLUMN_DESCRIPTION,
  COLUMN_PULSE, COLUMN_ICON, COLUMN_ACTIVE, COLUMN_SENSITIVE, NUM_COLUMNS =
    *(0..8).to_a
  DATA = [
    [false, 60_482, "Normal",     "scrollable notebooks and hidden tabs" ],
    [false, 60_620, "Critical",   "gdk_window_clear_area (gdkwindow-win32.c) is not thread-safe" ],
    [false, 50_214, "Major",      "Xft support does not clean up correctly" ],
    [true,  52_877, "Major",      "GtkFileSelection needs a refresh method. " ],
    [false, 56_070, "Normal",     "Can't click button after setting in sensitive" ],
    [true,  56_355, "Normal",     "GtkLabel - Not all changes propagate correctly" ],
    [false, 50_055, "Normal",     "Rework width/height computations for TreeView" ],
    [false, 58_278, "Normal",     "gtk_dialog_set_response_sensitive  doesn't work" ],
    [false, 55_767, "Normal",     "Getters for all setters" ],
    [false, 56_925, "Normal",     "Gtkcalender size" ],
    [false, 56_221, "Normal",     "Selectable label needs right-click copy menu" ],
    [true,  50_939, "Normal",     "Add shift clicking to GtkTextView" ],
    [false, 6_112,  "Enhancement","netscape-like collapsable toolbars" ],
    [false, 1,      "Normal",     "First bug :=)" ],
  ].collect do |ary|
    Bug.new(*ary)
  end

  def initialize(main_window)
    @window = Gtk::Window.new(:toplevel)
    @window.screen = main_window.screen
    @window.title = "List Store"
    @timeout = 0
    @window.signal_connect "delete-event" do
      @model = nil
      GLib::Source.remove(@timeout) unless @timeout.zero?
      @timeout = 0
      false
    end
    vbox = Gtk::Box.new(:vertical, 8)
    vbox.margin = 8
    @window.add(vbox)

    label = Gtk::Label.new(<<-EOF)
    This is the bug list (note: not based on real data, it would be nice to
    have a nice ODBC interface to bugzilla or so, though).
EOF
    vbox.pack_start(label, :expand => false, :fill => false, :padding => 0)
    sw = Gtk::ScrolledWindow.new(nil, nil)
    sw.shadow_type = :etched_in
    sw.set_policy(:never, :automatic)
    vbox.pack_start(sw, :expand => true, :fill => true, :padding => 0)

    # create tree model
    create_model

    # create tree view
    treeview = Gtk::TreeView.new(@model)
    treeview.search_column = COLUMN_DESCRIPTION
    sw.add(treeview)

    # add columns to the tree view
    add_columns(treeview)

    # finish and show
    @window.set_default_size(280, 250)
  end

  def run
    if !@window.visible?
      @window.show_all
      add_spinner

    else
      @window.destroy
      GLib::Source.remove(@tiemout) unless @timeout.zero?
      @timeout = 0
    end
    @window
  end

  private

  def create_model
    # create list store
    @model = Gtk::ListStore.new(TrueClass, Integer, String, String, Integer,
                                String, TrueClass, TrueClass)

    # add data to the list store
    DATA.each_with_index do |bug, i|
      icon_name = ""
      icon_name = "battery-caution-charging-symbolic" if i == 1 || i == 3
      sensitive = i == 3 ? false : true

      iter = @model.append
      iter.set_values([bug.fixed, bug.number, bug.severity, bug.description,
                       0, icon_name, false, sensitive])
    end
  end

  def add_columns(treeview)
    # column for fixed toggles
    renderer = Gtk::CellRendererToggle.new
    renderer.signal_connect("toggled") { |_cell, path| fixed_toggled(path) }
    column = Gtk::TreeViewColumn.new("Fixed?",
                                     renderer,
                                     "active" => COLUMN_FIXED)

    # set this column to a fixed sizing (of 50 pixels)
    column.sizing = :fixed
    column.fixed_width = 50
    treeview.append_column(column)

    # column for bug numbers
    renderer = Gtk::CellRendererText.new
    column = Gtk::TreeViewColumn.new("Bug number",
                                     renderer,
                                     "text" => COLUMN_NUMBER)
    column.sort_column_id = COLUMN_NUMBER
    treeview.append_column(column)

    # column for severities
    renderer = Gtk::CellRendererText.new
    column = Gtk::TreeViewColumn.new("Severity",
                                     renderer,
                                     "text" => COLUMN_SEVERITY)
    column.sort_column_id = COLUMN_SEVERITY
    treeview.append_column(column)

    # column for description
    renderer = Gtk::CellRendererText.new
    column = Gtk::TreeViewColumn.new("Description",
                                     renderer,
                                     "text" => COLUMN_DESCRIPTION)
    column.sort_column_id = COLUMN_DESCRIPTION
    treeview.append_column(column)

    # column for spinner
    renderer = Gtk::CellRendererSpinner.new
    column = Gtk::TreeViewColumn.new("Spinning",
                                     renderer,
                                     "pulse" => COLUMN_PULSE,
                                     "active" => COLUMN_ACTIVE)
    column.sort_column_id = COLUMN_PULSE
    treeview.append_column(column)

    # column for symbolic icon
    renderer = Gtk::CellRendererPixbuf.new
    column = Gtk::TreeViewColumn.new("Symbolic icon",
                                     renderer,
                                     "icon-name" => COLUMN_ICON,
                                     "sensitive" => COLUMN_SENSITIVE)
    column.sort_column_id = COLUMN_ICON
    treeview.append_column(column)
  end

  def fixed_toggled(path_str)
    path = Gtk::TreePath.new(path_str)

    # get toggled iter
    iter = @model.get_iter(path)
    fixed = iter[COLUMN_FIXED]

    # do something with the value
    fixed ^= 1

    # set new value
    iter[COLUMN_FIXED] = fixed
  end

  def add_spinner
    if @timeout.zero?
      @timeout = GLib::Timeout.add(80) do
        GLib::Source.remove(@timeout) unless @model
        iter = @model.iter_first
        pulse = iter[COLUMN_PULSE]

        pulse = pulse == GLib::MAXUINT ? 0 : pulse + 1
        iter[COLUMN_PULSE] = pulse
        iter[COLUMN_ACTIVE] = true

        true
      end
    end
  end
end
