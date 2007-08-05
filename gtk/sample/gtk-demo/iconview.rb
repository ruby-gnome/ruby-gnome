# Copyright (c) 2005 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
# $Id: iconview.rb,v 1.7 2007/08/05 15:41:31 mutoh Exp $
=begin
= Icon View (IconView)

The Gtk::IconView widget is used to display and manipulate icons.
It uses a Gtk::TreeModel for data storage, so the list store example
might be helpful.
=end
$KCODE="u"
require 'common'

module Demo
  class IconView < Demo::BasicWindow
    COL_PATH, COL_DISPLAY_NAME, COL_IS_DIR, COL_PIXBUF = (0..3).to_a

    def fill_store
      @store.clear
      Dir.glob(File.join(@parent, "*")).each do |path|
	is_dir = FileTest.directory?(path)
	iter = @store.append
        # set COL_DISPLAY_NAME first because changing an iter will trigger the
        # sort function; if we set something else first, the value of
        # COL_DISPLAY_NAME for this row will be "nil" and the sort function will fail
puts path
#puts GLib.convert(GLib.filename_to_utf8(path), "Shift_JIS", "UTF-8")
#puts File.basename(GLib.convert(GLib.filename_to_utf8(path), "Shift_JIS", "UTF-8"))
#        iter[COL_DISPLAY_NAME] = File.basename(GLib.filename_to_utf8(path))
        iter[COL_DISPLAY_NAME] = GLib.filename_to_utf8(path)
	iter[COL_PATH] = path
	iter[COL_IS_DIR] = is_dir
	iter[COL_PIXBUF] = is_dir ? @folder_pixbuf : @file_pixbuf
      end
    end

    def initialize
      super('Gtk::IconView demo')
      @file_pixbuf = Gdk::Pixbuf.new(Demo.find_file("gnome-fs-regular.png"))
      @folder_pixbuf = Gdk::Pixbuf.new(Demo.find_file("gnome-fs-directory.png"))

      @store = Gtk::ListStore.new(String, String, TrueClass, Gdk::Pixbuf)
      @parent = "/"

      @store.set_default_sort_func do |a, b|
        if !a[COL_IS_DIR] and b[COL_IS_DIR]
          1
        elsif a[COL_IS_DIR] and !b[COL_IS_DIR]
          -1
        else
          a[COL_DISPLAY_NAME] <=> b[COL_DISPLAY_NAME]
        end
      end
      @store.set_sort_column_id(Gtk::TreeSortable::DEFAULT_SORT_COLUMN_ID,
				Gtk::SORT_ASCENDING)

      fill_store
      set_default_size(650, 400)
      set_border_width(8)

      vbox = Gtk::VBox.new(false, 0)
      add(vbox)

      toolbar = Gtk::Toolbar.new
      vbox.pack_start(toolbar, false, false, 0)

      up_button = Gtk::ToolButton.new(Gtk::Stock::GO_UP)
      up_button.important = true
      up_button.sensitive = false
      toolbar.insert(-1, up_button)
      up_button.signal_connect("clicked") do
	@parent = File.dirname(@parent)
	fill_store
	up_button.sensitive = @parent != "/"
      end

      home_button = Gtk::ToolButton.new(Gtk::Stock::HOME)
      home_button.important = true
      toolbar.insert(-1, home_button)
      home_button.signal_connect("clicked") do
	@parent = GLib.home_dir
	fill_store
	up_button.sensitive = true
      end

      sw = Gtk::ScrolledWindow.new
      sw.shadow_type = Gtk::SHADOW_ETCHED_IN
      sw.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC)
      vbox.pack_start(sw, true, true, 0)

      iconview = Gtk::IconView.new(@store)
      iconview.selection_mode = Gtk::SELECTION_MULTIPLE
      iconview.text_column = COL_DISPLAY_NAME
      iconview.pixbuf_column = COL_PIXBUF
      iconview.signal_connect("item_activated") do |iview, path|
	iter = @store.get_iter(path)
	if iter[COL_DISPLAY_NAME]
	  @parent = iter[COL_PATH]
	  fill_store
	  up_button.sensitive = true
	end
      end
      sw.add(iconview)
      iconview.grab_focus
    end
  end
end

