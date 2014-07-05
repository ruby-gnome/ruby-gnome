# Copyright (c) 2003-2005 Ruby-GNOME2 Project Team
# This program is licenced under the same licence as Ruby-GNOME2.
#
# $Id: stock_browser.rb,v 1.5 2005/02/12 23:02:43 kzys Exp $
=begin
= Stock Item and Icon Browser

This source code for this demo doesn't demonstrate anything
particularly useful in applications. The purpose of the 'demo' is
just to provide a handy place to browse the available stock icons
and stock items.
=end
require 'common'

module Demo
  class StockBrowser < BasicWindow
    Item = Struct.new('Item',
      :stock_id, :label, :modifier, :keyval, :translation_domain)
    ItemInfo = Struct.new('ItemInfo',
      :id, :item, :small_icon, :const, :accel_str)
    ItemDisplay = Struct.new('ItemDisplay',
      :type_label, :const_label, :id_label, :label_accel_label,
      :icon_image)

    def initialize
      super('Stock Icons and Items')
      set_default_size(-1, 500)

      self.border_width = 8

      hbox = Gtk::Box.new(:horizontal, 8)
      add(hbox)

      sw = Gtk::ScrolledWindow.new
      sw.set_policy(:never, :automatic)
      hbox.pack_start(sw, :expand => false, :fill => false, :padding => 0)

      model = create_model

      treeview = Gtk::TreeView.new(model)
      sw.add(treeview)

      column = Gtk::TreeViewColumn.new
      column.title = 'Const'

      cell_renderer = Gtk::CellRendererPixbuf.new
      column.pack_start(cell_renderer, :expand => false)
      column.set_attributes(cell_renderer, "stock_id" => 1)

      cell_renderer = Gtk::CellRendererText.new
      column.pack_start(cell_renderer, :expand => true)
      column.set_cell_data_func(cell_renderer) do |column, cell, model, iter|
        cell.text = iter[0].const
      end

      treeview.append_column(column)

      cell_renderer = Gtk::CellRendererText.new
      treeview.insert_column(-1, 'Label', cell_renderer) do |col, cell, model, iter|
        info = iter[0]
        cell.text = info.item.label if info
      end

      cell_renderer = Gtk::CellRendererText.new
      treeview.insert_column(-1, 'Accel', cell_renderer) do |col, cell, model, iter|
        info = iter[0]
        cell.text = info.accel_str if info
      end

      cell_renderer = Gtk::CellRendererText.new
      treeview.insert_column(-1, 'ID', cell_renderer) do |col, cell, model, iter|
        info = iter[0]
        cell.text = info.id if info
      end

      align = Gtk::Alignment.new(0.5, 0.0, 0.0, 0.0)
      hbox.pack_end(align, :expand => false, :fill => false, :padding => 0)

      frame = Gtk::Frame.new('Selected Item')
      align.add(frame)

      vbox = Gtk::Box.new(:vertical, 8)
      vbox.border_width = 4
      frame.add(vbox)

      display = ItemDisplay.new
      class << treeview
        @display = nil
        attr_accessor :display
      end
      treeview.display = display

      display.type_label = Gtk::Label.new
      display.const_label = Gtk::Label.new
      display.id_label = Gtk::Label.new
      display.label_accel_label = Gtk::Label.new
      display.icon_image = Gtk::Image.new # empty image

      vbox.pack_start(display.type_label, :expand => false, :fill => false, :padding => 0)

      vbox.pack_start(display.icon_image, :expand => false, :fill => false, :padding => 0)

      vbox.pack_start(display.label_accel_label, :expand => false, :fill => false, :padding => 0)
      vbox.pack_start(display.const_label, :expand => false, :fill => false, :padding => 0)
      vbox.pack_start(display.id_label, :expand => false, :fill => false, :padding => 0)

      selection = treeview.selection
      selection.mode = :single

      selection.signal_connect('changed') do |s|
        selection_changed(s)
      end
    end

    def create_model
      store = Gtk::ListStore.new(ItemInfo, String, String, String)

      list_ids = Gtk::Stock.ids
      list_ids.sort! do |a, b|
        a.to_s <=> b.to_s
      end
      list_ids.each do |stock_id|
        info = ItemInfo.new

        info.id = stock_id

        begin
          info.item = Item.new(*Gtk::Stock.lookup(stock_id))
        rescue ArgumentError
          info.item = Item.new
        end

        # only show icons for stock IDs that have default icons
        icon_set = Gtk::IconFactory.lookup_default(info.id.to_s)
        if icon_set
          # See what sizes this stock icon really exists at
          sizes = icon_set.sizes

          # Use menu size if it exists, otherwise first size found
          size = sizes.find do |s| s == :menu end || sizes.first

          info.small_icon = render_icon_pixbuf(info.id, size)

          unless size == :menu
            # Make the result the proper size for our thumbnail
            w, h = Gtk::IconSize.lookup(size)

            scaled = info.small_icon.scale(w, h, Gdk::Pixbuf::INTERP_BILINEAR)
            info.small_icon = scaled
          end

        else
          info.small_icon = nil
        end

        if info.item.keyval
          info.accel_str = Gtk::Accelerator.to_name(info.item.keyval,
                      info.item.modifier)
        else
          info.accel_str = ''
        end

        info.const = id_to_const(info.id)

        iter = store.append
        iter[0] = info
        iter[1] = info.id
      end

      return store
    end

    def selection_changed(selection)
      treeview = selection.tree_view
      display = treeview.display

      iter = selection.selected
      info = iter[0]

      display.type_label.label = if info.small_icon and info.item.label
                                   'Icon and Item'
                                 elsif info.small_icon
                                   'Icon Only'
                                 elsif info.item.label
                                   'Item Only'
                                 else
                                   '???????'
                                 end
      display.const_label.label = info.const
      display.id_label.label = info.id

      if info.item.label
        str = "%s %s" % [info.item.label, info.accel_str]
        display.label_accel_label.set_text(str, true)
      else
        display.label_accel_label.set_text('')
      end

      if info.small_icon
        display.icon_image.set_from_stock(info.id, get_largest_size(info.id))
      else
        display.icon_image.pixbuf = nil
      end
    end

    def get_largest_size(symbol)
      set = Gtk::IconFactory.lookup_default(symbol.id2name)
      best_size = :invalid
      best_pixels = 0

      sizes = set.sizes
      sizes.collect! do |i|
        width, height = Gtk::IconSize.lookup(i)
        [i, width * height]
      end
      return (sizes.max do |a, b| a[1] <=> b[1] end)[0]
    end

    def id_to_const(id)
      const = ''

      s = id.id2name
      if s =~ /^gtk-/
        const += 'Gtk::Stock::'
        s[0,4] = ''
      end

      const += s.upcase.gsub(/-/, '_')

      const
    end
  end
end
