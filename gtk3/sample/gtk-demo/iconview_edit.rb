# Copyright (c) 2015-2016 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
=begin
=  Icon View/Editing and Drag-and-Drop

 The GtkIconView widget supports Editing and Drag-and-Drop.
 This example also demonstrates using the generic GtkCellLayout
 interface to set up cell renderers in an icon view.
=end
class IconviewEditDemo
  COL_TEXT = 0
  NUM_COLS = 1
  def initialize(main_window)
    @window = Gtk::Window.new(:toplevel)
    @window.screen = main_window.screen
    @window.title = "Editing and Drag-and-drop"

    initialize_store
    initialize_icon_view

    icon_view_add_renderer_pixbuf
    icon_view_add_renderer_text

    @window.add(@icon_view)
  end

  def run
    if !@window.visible?
      @window.show_all
    else
      @window.destroy
    end
    @window
  end

  def initialize_store
    @store = Gtk::ListStore.new(String)

    %w(Red Green Blue Yellow).each do |color|
      @store.append.set_values([color])
    end
  end

  def initialize_icon_view
    @icon_view = Gtk::IconView.new(:model => @store)
    @icon_view.selection_mode = :single
    @icon_view.item_orientation = :horizontal
    @icon_view.columns = 2
    @icon_view.reorderable = true
  end

  def icon_view_add_renderer_pixbuf
    renderer = Gtk::CellRendererPixbuf.new
    @icon_view.pack_start(renderer, true)

    @icon_view.set_cell_data_func(renderer) do |_layout, cell_renderer, model, iter|
      text = model.get_value(iter, COL_TEXT)
      if text
        begin
          color = Gdk::RGBA.parse(text)
        rescue ArgumentError
          color = nil
        end
        if color
          pixel = (color.red * 255).to_i << 24 |
                  (color.green * 255).to_i << 16 |
                  (color.blue * 255).to_i << 8 |
                  (color.alpha * 255).to_i
        else
          pixel = 0
        end
        pixbuf = GdkPixbuf::Pixbuf.new(:colorspace => :rgb, :has_alpha => true,
                                       :bits_per_sample => 8,
                                       :width => 24, :height => 24)
        pixbuf.fill!(pixel)
        cell_renderer.set_property("pixbuf", pixbuf)
      end
    end
  end

  def icon_view_add_renderer_text
    renderer = Gtk::CellRendererText.new
    @icon_view.pack_start(renderer, true)
    renderer.set_property("editable", true)
    renderer.signal_connect("edited") do |_cell, path_string, text|
      model = @icon_view.model
      path = Gtk::TreePath.new(path_string)
      iter = model.get_iter(path)
      iter[COL_TEXT] = text
    end

    @icon_view.set_attributes(renderer, "text" => COL_TEXT)
  end
end
