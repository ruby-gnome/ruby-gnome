=begin
  iconview.rb - Icon View

  The Gtk::IconView widget is used to display and manipulate icons.  
  It uses a Gtk::TreeModel for data storage, so the list store example 
  might be helpful.

  Copyright (c) 2005 Ruby-GNOME2 Project Team
  This program is licenced under the same licence as Ruby-GNOME2.

  $Id: iconview.rb,v 1.1 2005/01/03 18:55:02 mutoh Exp $
=end
require 'common'

module Demo
  class IconView < Demo::BasicWindow

    def fill_store
      @store.clear
      Dir.glob(File.join(@parent, "*")).each do |path|
	is_dir = FileTest.directory?(path)
	iter = @store.append
	iter[0] = path
	iter[1] = GLib.filename_to_utf8(File.basename(path))
	iter[2] = is_dir
	iter[3] = is_dir ? @folder_pixbuf : @file_pixbuf
      end
    end

    def initialize
      super('Gtk::IconView demo')
      @file_pixbuf = Gdk::Pixbuf.new(Demo.find_file("gnome-fs-regular.png"))
      @folder_pixbuf = Gdk::Pixbuf.new(Demo.find_file("gnome-fs-directory.png"))

      @store = Gtk::ListStore.new(String, String, TrueClass, Gdk::Pixbuf)
      @parent = "/"

      @store.set_default_sort_func{|a, b|
	if (!a[2] && b[2])
	  ret = 1
	elsif (a[2] && !b[2])
	  ret = -1
	else
	  ret = a[0] <=> b[0]
	end
	ret
      }
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
      iconview.text_column = 1
      iconview.pixbuf_column = 3
      iconview.signal_connect("item_activated") do |iview, path|
	iter = @store.get_iter(path)
	if iter[2]
	  @parent = iter[0]
	  fill_store
	  up_button.sensitive = true
	end
      end
      sw.add(iconview)
      iconview.grab_focus
      
    end
  end
end

