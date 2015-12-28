# Copyright (c) 2015 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
=  Icon View/Editing and Drag-and-Drop

 The GtkIconView widget supports Editing and Drag-and-Drop.
 This example also demonstrates using the generic GtkCellLayout
 interface to set up cell renderers in an icon view.
=end
module IconviewEditDemo
  COL_TEXT = 0
  NUM_COLS = 1
  def self.run_demo(main_window)
    window = Gtk::Window.new(:toplevel)
    window.screen = main_window.screen
    window.set_title("Editing and Drag-and-drop")

    store = create_store
    fill_store(store)

    icon_view = Gtk::IconView.new(:model => store)
    icon_view.set_selection_mode(:single)
    icon_view.set_item_orientation(:horizontal)
    icon_view.set_columns(2)
    icon_view.set_reorderable(true)

    renderer = Gtk::CellRendererPixbuf.new
    icon_view.pack_start(renderer, true)

    icon_view.set_cell_data_func(renderer) do |_layout, cell_renderer, model, iter|
      text = model.get_value(iter, COL_TEXT)
      if text
        color = Gdk::RGBA.parse(text)
        pixel = nil
        if color
          pixel = (color.red * 255).to_i << 24 |
                  (color.green * 255).to_i << 16 |
                  (color.blue * 255).to_i << 8 |
                  (color.alpha * 255).to_i
        end
        pixbuf = Gdk::Pixbuf.new(Gdk::Pixbuf::COLORSPACE_RGB, true, 8, 24, 24)
        pixbuf.fill!(pixel) if pixel
        cell_renderer.set_property("pixbuf", pixbuf)
      end
    end

    renderer = Gtk::CellRendererText.new
    icon_view.pack_start(renderer, true)
    renderer.set_property("editable", true)
    renderer.signal_connect("edited") do |_cell, path_string, text|
      model = icon_view.model
      path = Gtk::TreePath.new(path_string)
      iter = model.get_iter(path)
      iter[COL_TEXT] = text
    end

    icon_view.set_attributes(renderer, "text" => COL_TEXT)
    window.add(icon_view)

    if !window.visible?
      window.show_all
    else
      window.destroy
    end
    window
  end

  def self.create_store
    store = Gtk::ListStore.new(String)
    store
  end

  def self.fill_store(store)
    %w(Red Green Blue Yellow).each do |color|
      store.append.set_values([color])
    end
  end
end
