# Copyright (c) 2016 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
=  Icon View/Icon View Basics

 The GtkIconView widget is used to display and manipulate icons.
 It uses a GtkTreeModel for data storage, so the list store
 example might be helpful.
=end
class IconviewDemo
  COL_PATH, COL_DISPLAY_NAME, COL_PIXBUF, COL_IS_DIRECTORY = (0..3).to_a
  FOLDER_NAME = "/iconview/gnome-fs-directory.png".freeze
  FILE_NAME = "/iconview/gnome-fs-regular.png".freeze

  def initialize(main_window)
    @window = Gtk::Window.new(:toplevel)
    @window.screen = main_window.screen
    @window.title = "Icon View Basics"
    @window.set_default_size(650, 400)
    load_pixbufs

    vbox = Gtk::Box.new(:vertical, 0)
    @window.add(vbox)

    tool_bar = Gtk::Toolbar.new
    vbox.pack_start(tool_bar, :expand => false, :fill => false, :padding => 0)

    up_button = Gtk::ToolButton.new
    up_button.label = "_Up"
    up_button.use_underline = true
    up_button.icon_name = "go-up"
    up_button.is_important = true
    up_button.sensitive = false
    tool_bar.insert(up_button, -1)

    home_button = Gtk::ToolButton.new
    home_button.label = "_Home"
    home_button.use_underline = true
    home_button.icon_name = "go-home"
    home_button.is_important = true
    tool_bar.insert(home_button, -1)

    sw = Gtk::ScrolledWindow.new
    sw.shadow_type = :etched_in
    sw.set_policy(:automatic, :automatic)
    vbox.pack_start(sw, :expand => true, :fill => true, :padding => 0)

    # Create the store and fill it with the contents of '/'
    @parent = "/"
    create_store
    fill_store

    icon_view = Gtk::IconView.new(:model => @store)
    icon_view.selection_mode = :multiple

    # Connect to the "clicked" signal of the "Up" tool button
    up_button.signal_connect "clicked" do
      @parent = File.dirname(@parent)
      fill_store

      # Maybe de-sensitive the up button
      up_button.sensitive = @parent == "/"
    end

    # Connect to the "clicked" signal of the "Home" too button
    home_button.signal_connect "clicked" do
      @parent = ENV["HOME"]
      fill_store
      up_button.sensitive = true
    end

    # We now set which model columns that correspond to the text
    # and pixbbuf of each of item
    icon_view.text_column = COL_DISPLAY_NAME
    icon_view.pixbuf_column = COL_PIXBUF

    # Connec the "item-activated" signal
    icon_view.signal_connect "item-activated" do |_widget, tree_path|
      iter = @store.get_iter(tree_path)
      path = iter[COL_PATH]
      directory = iter[COL_IS_DIRECTORY]
      if directory
        @parent = path
        fill_store
        up_button.sensitive = true
      end
    end
    sw.add(icon_view)
    icon_view.grab_focus
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

  def load_pixbufs
    # Loads the images for the demo and returns whether the operation succeeded
    return if @file_pixbuf

    @file_pixbuf = GdkPixbuf::Pixbuf.new(:resource => FILE_NAME)
    return false unless @file_pixbuf

    @folder_pixbuf = GdkPixbuf::Pixbuf.new(:resource => FOLDER_NAME)
    return false unless @folder_pixbuf
  end

  def create_store
    @store = Gtk::ListStore.new(String, String, GdkPixbuf::Pixbuf, TrueClass)

    # Set sort column and function
    @store.set_default_sort_func do |_model, a, b|
      sort_function(a, b)
    end
    @store.set_sort_column_id(Gtk::TREE_SORTABLE_DEFAULT_SORT_COLUMN_ID,
                              :ascending)
  end

  def fill_store
    @store.clear
    Dir.glob(File.join(@parent, "*")).each do |path|
      is_dir = FileTest.directory?(path)
      iter = @store.append
      iter[COL_DISPLAY_NAME] = File.basename(path) || "-"
      iter[COL_PATH] = path
      iter[COL_PIXBUF] = is_dir ? @folder_pixbuf : @file_pixbuf
      iter[COL_IS_DIRECTORY] = is_dir
    end
  end

  def sort_function(a, b)
    if !a[COL_IS_DIRECTORY] and b[COL_IS_DIRECTORY]
      1
    elsif a[COL_IS_DIRECTORY] and !b[COL_IS_DIRECTORY]
      -1
    elsif !a[COL_DISPLAY_NAME] and b[COL_DISPLAY_NAME]
      1
    elsif a[COL_DISPLAY_NAME] and !b[COL_DISPLAY_NAME]
      -1
    else
      a[COL_DISPLAY_NAME] <=> b[COL_DISPLAY_NAME]
    end
  end
end
